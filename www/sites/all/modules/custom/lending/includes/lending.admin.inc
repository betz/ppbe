<?php // -*- drupal -*-
  /**
   * @file
   * Administration page callbacks for the lending module.
   */

  /**
   * Form builder. Configure lending.
   *
   * @ingroup forms
   * @see system_settings_form().
   */

function lending_admin_settings() {
  // Get an array of node types with internal names as keys and
  // "friendly names" as values. E.g.,
  // array('page' => 'Page', 'story' => 'Story')
  $options = node_get_types('names');
  $form['lending_node_types'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Users may lending these content types'),
    '#options' => $options,
    '#default_value' => variable_get('lending_node_types', array()),
    '#description' => t('These types of content can be checked out'),
  );
  return system_settings_form($form);
}
