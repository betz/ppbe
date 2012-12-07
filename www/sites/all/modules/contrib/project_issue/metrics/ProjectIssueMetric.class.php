<?php

/**
 * @file
 * Base class for Project issue metrics.
 */

class ProjectIssueMetric extends SamplerMetric {

  public function projectIssueMetricInitProjects($data_types) {
    // Load options.
    $options = $this->currentSample->options;
    if (!empty($options['object_ids'])) {
      $object_ids = $options['object_ids'];
    }
    else {
      $object_ids = $this->trackObjectIDs();
    }
    foreach ($object_ids as $object_id) {
      $this->currentSample->values[$object_id] = $data_types;
    }
  }

  public function trackObjectIDs() {
    $types = project_project_node_types();
    $nids = array();
    $projects = db_query('SELECT nid FROM {node} WHERE type IN (:types)', array(':types' => $types));
    foreach ($projects as $project) {
      $nids[] = $project->nid;
    }
    return $nids;
  }
}
