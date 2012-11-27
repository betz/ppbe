<?php

/**
 * @file
 * Code related to node/add/project-release and friends.
 */

/**
 * Form builder for a simple form to select a project when creating a new
 * release (as the first "page", but this is not really a multi-page form).
 */
function project_release_pick_project_form($form, &$form_state, $node_type) {
  // Fetch a list of all projects with releases.
  $projects = project_projects_select_options(array(array('field' => 'field_project_has_releases')));
  if (count($projects) === 0) {
    drupal_set_message(t('You do not have access to any projects.'), 'error');
  }

  $form_state['node_type'] = $node_type;

  $form['project_id'] = array(
    '#type' => 'select',
    '#title' => t('Project'),
    '#options' => $projects,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Next'),
  );
  return $form;
}

function project_release_pick_project_form_validate($form, &$form_state) {
  if (!empty($form_state['values']['project_id'])) {
    $node = node_load($form_state['values']['project_id']);
    if (empty($node) || !project_node_is_project($node)) {
      form_set_error('project_id', t('Invalid project selected.'));
    }
    else {
      $form_state['project_machine_name'] = $node->field_project_machine_name[LANGUAGE_NONE][0]['value'];
    }
  }
}

function project_release_pick_project_form_submit($form, &$form_state) {
  $type_url_str = str_replace('_', '-', $form_state['node_type']);
  $form_state['redirect'] = 'node/add/' . $type_url_str . '/' . $form_state['project_machine_name'];
}
