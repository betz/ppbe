<?php

/**
 * @file
 *   Base class for Project release metrics.
 */

class ProjectReleaseMetric extends SamplerMetric {

  public function projectReleaseMetricInitProjects($data_types) {
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
    $nids = array();
    $projects = db_query("SELECT DISTINCT(pp.nid) FROM {project_projects} pp INNER JOIN {project_release_nodes} prn ON pp.nid = prn.pid");
    while ($project = db_fetch_object($projects)) {
      $nids[] = $project->nid;
    }
    return $nids;
  }
}
