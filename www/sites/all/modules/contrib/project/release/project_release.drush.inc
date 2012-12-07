<?php

/**
 * @file
 * Drush command for packaging files for release nodes.
 *
 * @author Derek Wright (http://drupal.org/user/46549)
 */

function project_release_drush_command() {
  return array(
    'release-package' => array(
      'description' => 'Package files for release nodes.',
      'arguments' => array(
        'type' => '"branch" or "tag", required.',
        'project_id' => 'Project, either a Node ID or machine name. Omit to build all projects.'
      ),
      'required-arguments' => TRUE,
    ),
  );
}

function project_release_drush_help($section) {
  switch ($section) {
    case 'meta:project_release:title':
      return dt('Project release commands');

    case 'drush:release-package':
      return dt('Package files for release nodes.');
  }
}

function drush_project_release_release_package_validate($type = NULL, $project_id = NULL) {
  if ($type !== 'tag' && $type !== 'branch') {
    return drush_set_error('INVALID_TYPE', dt('First type must be "branch" or "tag".'));
  }
}

function drush_project_release_release_package($type, $project_id) {
  // Force the right umask while this script runs, so that everything is created
  // with sane file permissions.
  umask(0022);

  // Load the include file for packager-related helper functions.
  module_load_include('inc', 'project_release', 'includes/packager');

  // Find the project.
  $project_node = project_load($project_id);
  if ($project_node === FALSE) {
    return drush_set_error('INVALID_PROJECT', dt('ERROR: Project ID @id not found', array('@id' => $project_id)));
  }
  if ($project_node->status != NODE_PUBLISHED) {
    return drush_set_error('UNPUBLISHED_PROJECT', dt('ERROR: Project @id not published', array('@id' => $project_id)));
  }
  if (!$project_node->field_project_has_releases[$project_node->language][0]['value']) {
    return drush_set_error('NO_RELEASES_PROJECT', dt('ERROR: Project @id does not have releases', array('@id' => $project_id)));
  }

  $num_built = 0;
  $num_considered = 0;

  // Find release nodes.
  $query = new EntityFieldQuery();
  $result = $query->entityCondition('entity_type', 'node')
    ->entityCondition('bundle', 'project_release')
    ->fieldCondition('field_release_project', 'target_id', $project_node->nid)
    ->fieldCondition('field_release_build_type', 'value', $type === 'tag' ? 'static' : 'dynamic')
    ->execute();
  if (empty($result)) {
    drush_log(dt('No !type releases found for @id.', array('!type' => $type, '@id' => $project_id)), 'warning');
    return;
  }

  // If needed, prod module_implements() to recognize our
  // project_release_watchdog() implementation.
  if (!in_array('project_release', module_implements('watchdog'))) {
    module_implements('watchdog', FALSE, TRUE);
  }

  foreach (array_keys($result['node']) as $release_nid) {
    // We don't want to waste too much RAM by leaving all these loaded nodes
    // in RAM, so we reset the node_load() cache each time we call it.
    $release_node = node_load($release_nid, NULL, TRUE);
    if (empty($release_node)) {
      watchdog('package_error', "Can't load release node for release ID %nid", array('%nid' => $release_nid), WATCHDOG_ERROR);
      continue;
    }

    // If file is not empty, a tag release is already packaged.
    if ($type === 'tag' && !empty($release_node->field_release_files)) {
      continue;
    }
    // If published and file is not empty, a branch release is already
    // packaged. Published releases may be repackaged.
    if ($type === 'branch' && $release_node->status != NODE_PUBLISHED && empty($release_node->field_release_files)) {
      continue;
    }

    // Find the packager plugin.
    $packager = project_release_get_packager_plugin($release_node, drush_tempdir());
    if ($packager === FALSE) {
      watchdog('package_error', "Can't find packager plugin to use for %release", array('%release' => $release_node->title), WATCHDOG_ERROR);
      continue;
    }

    // Delete existing log messages.
    db_delete('project_release_package_errors')
      ->condition('nid', $release_node->nid)
      ->execute();
    project_release_watchdog(NULL, $release_node->nid);

    $files = array();
    $rval = $packager->createPackage($files);
    $num_considered += 1;

    switch ($rval) {
      case 'success':
      case 'rebuild':
        project_release_packager_update_node($release_node, $files);
        module_invoke_all('project_release_create_package', $project_node, $release_node);
        $num_built += 1;
        if ($rval === 'rebuild') {
          $msg = '%release_title has changed, re-packaged.';
        }
        else {
          $msg = 'Packaged %release_title.';
        }
        watchdog('package_' . $type, $msg, array('%release_title' => $release_node->title), WATCHDOG_INFO, l(t('View'), 'node/' . $release_node->nid));
        break;
    }

    // Write any log entries.
    project_release_watchdog(NULL, FALSE);
  }

  if ($num_built > 0 || $type === 'branch') {
    watchdog('package_' . $type, "Done packaging !type releases for @project_short_name: !num_built built, !num_considered considered.", array('@project_short_name' => $project_node->field_project_machine_name[$project_node->language][0]['value'], '!type' => $type, '!num_built' => $num_built, '!num_considered' => $num_considered));
  }
}

/**
 * Drush-only implementation of hook_watchdog(). Write out messages to
 * project_release_package_errors table.
 *
 * @param $log_entry
 *   An associative array, see hook_watchdog() for keys.
 * @param $new_nid
 *   Set release nid for log entries to be stored with. FALSE to reset and
 *   write out any entries.
 */
function project_release_watchdog($log_entry = NULL, $new_nid = NULL) {
  static $nid = FALSE;
  static $messages = array();

  if (is_null($new_nid)) {
    // Invoked via hook_watchdog(), store message if there is a current node.
    if ($nid !== FALSE && $log_entry['severity'] < WATCHDOG_NOTICE) {
      $messages[] = t($log_entry['message'], $log_entry['variables']);
    }
  }
  else {
    // Clearing out nid, write out messages.
    if ($new_nid === FALSE && !empty($messages)) {
      db_insert('project_release_package_errors')->fields(array(
        'nid' => $nid,
        'messages' => serialize($messages),
      ))->execute();
      $messages = array();
    }

    // Store new nid.
    $nid = $new_nid;
  }
}
