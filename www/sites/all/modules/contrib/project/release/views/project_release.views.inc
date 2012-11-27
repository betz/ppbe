<?php

/**
 * @file
 * Provides support for Views integration.
 */

/**
 * Implements hook_views_handlers().
 */
function project_release_views_handlers() {
  return array(
    'info' => array(
      'path' => drupal_get_path('module', 'project_release') .'/views/handlers',
    ),
    'handlers' => array(
      'project_release_handler_filter_version' => array(
        'parent' => 'views_handler_filter_field_list',
      ),
    ),
  );
}

/**
 * Implements hook_views_data_alter().
 */
function project_release_views_data_alter(&$data) {
  // Use our custom project-argument-aware handler for version.
  $data['field_data_field_issue_version']['field_issue_version_value']['filter']['handler'] = 'project_release_handler_filter_version';
  $data['field_revision_field_issue_version']['field_issue_version_value']['filter']['handler'] = 'project_release_handler_filter_version';
}
