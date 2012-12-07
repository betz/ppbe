<?php

/**
 * @file
 * Class for opened_vs_closed_by_category metric.
 */

class ProjectIssueOpenedVsClosedByCategory extends ProjectIssueMetric {

  public function computeSample() {
    // Load options.
    $options = $this->currentSample->options;

    // Initialize the projects array.
    $data_types = $this->newProjectOpenedVsClosed();
    $this->projectIssueMetricInitProjects($data_types);

    // The initial arguments are the 'open' project issue status options.
    $open_states = project_issue_open_states();

    // @todo Until http://drupal.org/node/214347 is resolved, we don't want
    // the 'fixed' status in the list of open statuses, so unset it here.
    $open_states = array_diff($open_states, array(PROJECT_ISSUE_STATE_FIXED));

    // Restrict to only the passed nids.
    $ids = array();
    if (!empty($options['object_ids'])) {
      $ids = $options['object_ids'];
    }

    // Use the historical calculation if requested.
    if (isset($options['historical']) && $options['historical'] == '1') {
      $this->buildSampleResultsHistorical($open_states, $ids);
    }
    // Otherwise, use the more efficient 'current' calculation.
    else {
      // Build total open issues counts.
      $this->buildSampleResultsCurrent('open', $open_states, $ids);
      // Build total closed issues counts.
      $this->buildSampleResultsCurrent('closed', $open_states, $ids);
    }

    // Add in total counts across all categories.
    foreach ($this->currentSample->values as $project_id => $values_array) {
      $this->currentSample->values[$project_id]['total_open'] = $values_array['bug_open'] + $values_array['feature_open'] + $values_array['task_open'] + $values_array['support_open'];
      $this->currentSample->values[$project_id]['total_closed'] = $values_array['bug_closed'] + $values_array['feature_closed'] + $values_array['task_closed'] + $values_array['support_closed'];
    }
  }

  /**
   * Builds the current values for total open/closed issues per project.
   *
   * @param $state
   *   Issue state, 'open' or 'closed'.
   * @param $open_states
   *   An array of state IDs that are considered 'open'.
   * @param $where
   *   Additional filters for the query.
   * @param $args
   *   Query arguments.
   */
  protected function buildSampleResultsCurrent($state, $open_states, $ids) {
    // Determine IN or NOT IN based on the state we're querying for.
    $in = $state == 'open' ? 'IN' : 'NOT IN';

    // Pull all issues grouped by project/category -- this query will miss any
    // projects that don't have issues, but we wouldn't want to show any data
    // for them, anyways.
    $query = db_select('node', 'n')
      ->condition('n.type', project_issue_issue_node_types());

    $query->innerJoin('field_data_field_project', 'p', "n.nid = p.entity_id AND p.entity_type = 'node'");
    $query->innerJoin('field_data_field_issue_status', 's', "n.nid = s.entity_id AND s.entity_type = 'node'");
    $query->innerJoin('field_data_field_issue_category', 'c', "n.nid = c.entity_id AND c.entity_type = 'node'");

    // Work around a data glitch -- some issues are currently pointing at invalid projects.
    $query->innerJoin('node', 'nn', 'p.field_project_target_id = nn.nid');

    $query->addField('p', 'field_project_target_id', 'pid');
    $query->addField('c', 'field_issue_category_value', 'category');
    $query->addExpression('COUNT(n.nid)', 'count');
    $query->groupBy('p.field_project_target_id');
    $query->groupBy('c.field_issue_category_value');

    $query->condition('s.field_issue_status_value', $open_states, $in);

    // Restrict to only the passed projects.
    if (!empty($ids)) {
      $query->condition('p.field_project_target_id', $ids);
    }

    $result = $query->execute();
    // @todo When http://drupal.org/node/115553 lands, this hard-coded list of
    // categories will need to be handled differently.
    $categories = array(1 => 'bug', 3 => 'feature', 2 => 'task', 4 => 'support');
    foreach ($result as $row) {
      // Add the total count for the category to the values array.
      if (isset($categories[$row->category])) {
        $this->currentSample->values[$row->pid][$categories[$row->category] . "_{$state}"] = (int)$row->count;
      }
    }
  }

  /**
   * Builds the historical values for total open/closed issues per project.
   *
   * @param $open_states
   *   An array of state IDs that are considered 'open'.
   * @param $where
   *   Additional filters for the query.
   * @param $args
   *   Query arguments.
   */
  protected function buildSampleResultsHistorical($open_states, $ids) {
    // Load current sample.
    $sample = $this->currentSample;

    $endstamp = $sample->sample_endstamp;

    // Pull last possible issue nid for the end of the period being measured.
    $max_nid = db_query('SELECT MAX(nid) FROM {node} WHERE created <= :created', array(':created' => $endstamp))->fetchField();

    if (!empty($max_nid)) {
      // Subquery to calculate the timestamp of the applicable revision
      $tsquery = db_select('node_revision', 'sv');
      $tsquery->condition('sv.timestamp', $endstamp, '<=');
      $tsquery->addField('sv', 'nid', 'nid');
      $tsquery->addExpression('MAX(sv.timestamp)', 'timestamp');

      $query = db_select($tsquery, 'sq');
      $query->innerJoin('node_revision', 'r', 'sq.nid = r.nid AND sq.timestamp = r.timestamp');
      $query->innerJoin('field_revision_field_project',        'p', "r.nid = p.entity_id AND r.vid = p.revision_id AND p.entity_type = 'node'");
      $query->innerJoin('field_revision_field_issue_status',   's', "r.nid = s.entity_id AND r.vid = s.revision_id AND s.entity_type = 'node'");
      $query->innerJoin('field_revision_field_issue_category', 'c', "r.nid = c.entity_id AND r.vid = c.revision_id AND c.entity_type = 'node'");
      $query->innerJoin('node', 'n', 'n.nid = r.nid');
      $query->condition('n.type', project_issue_issue_node_types());
      $query->condition('n.nid', $max_nid, '<=');
      $query->groupBy('n.nid');
      $query->groupBy('p.field_project_target_id');
      $query->addField('n', 'nid', 'nid');
      $query->addField('p', 'field_project_target_id', 'pid');
      $query->addField('s', 'field_issue_status_value', 'sid');
      $query->addField('c', 'field_issue_category_value', 'category');

      // Restrict to only the passed projects.
      if (!empty($ids)) {
        $query->condition('p.field_project_target_id', $ids);
      }

      $issues = $query->execute();
      // @todo When http://drupal.org/node/115553 lands, this hard-coded list of
      // categories will need to be handled differently.
      $categories = array(1 => 'bug', 3 => 'feature', 2 => 'task', 4 => 'support');
      foreach ($issues as $issue) {
        $state = in_array($issue->sid, $open_states) ? 'open' : 'closed';

        // Add to the total count for the category in the values array.
        if (isset($categories[$issue->category])) {
          $this->currentSample->values[$issue->pid][$categories[$issue->category] . "_{$state}"]++;
        }
      }

      // Clean up potentially huge arrays that are sucking memory.
      unset ($issues);
    }
    // The end of the sample is so early there aren't even any nodes created --
    // just bail on the entire sample.
    else {
      return FALSE;
    }
  }

  /**
   * Builds the starting values for a project for this metric.
   *
   * @return
   *   An associative array, keys are metric values, values are the default
   *   starting count for the metric values.
   */
  protected function newProjectOpenedVsClosed() {
    return array(
      'bug_open' => 0,
      'feature_open' => 0,
      'task_open' => 0,
      'support_open' => 0,
      'bug_closed' => 0,
      'feature_closed' => 0,
      'task_closed' => 0,
      'support_closed' => 0,
      'total_open' => 0,
      'total_closed' => 0,
    );
  }
}
