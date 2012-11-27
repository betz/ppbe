<?php

/**
 * @file Code to define the default project_issue node type and its fields.
 */

/**
 * Create a default node type for project_issue nodes.
 */
function _project_issue_create_issue_node_type() {
  $t = get_t();

  $type = array(
    'type' => 'project_issue',
    'name' => $t('Issue'),
    'base' => 'node_content',
    'description' => $t('An issue that can be tracked, such as a bug report, feature request, or task.'),
    'has_title' => '1',
    'title_label' => $t('Title'),
    'custom' => 1,
    'modified' => 1,
    'locked' => 0,
    'help' => '',
  );

  $type = node_type_set_defaults($type);
  node_type_save($type);
  node_add_body_field($type);

  // Define node workflow and comment settings.
  variable_set('node_options_project_issue', array('status', 'revision'));
  variable_set('comment_project_issue', COMMENT_NODE_OPEN);
  variable_set('comment_default_per_page_project_issue', '300');
  variable_set('comment_default_mode_project_issue', COMMENT_MODE_FLAT);
  variable_set('comment_form_location_project_issue', COMMENT_FORM_BELOW);

  // Tell project_issue that this node type is an issue node type.
  variable_set('project_behavior_project_issue', 'project_issue');

  if (module_exists('flag_tracker')) {
    // Setup the the flag tracker flag to use our flag.
    variable_set('flag_tracker_track_flag_project_issue', 'project_issue_follow');
    variable_set('flag_tracker_show_followers_project_issue', 1);
  }

  // Define default fields and field instances.

  // Status
  $field = array(
    'cardinality' => '1',
    'entity_types' => array(),
    'field_name' => 'field_issue_status',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => $t('Active'),
        13 => $t('Needs work'),
        8 => $t('Needs review'),
        2 => $t('Fixed'),
        4 => $t('Postponed'),
        3 => $t('Closed (duplicate)'),
        5 => $t("Closed (won't fix)"),
        6 => $t("Closed (works as designed)"),
        18 => $t("Closed (cannot reproduce)"),
        7 => $t("Closed (fixed)"),
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => '0',
    'type' => 'list_integer',
  );
  if (!field_info_field('field_issue_status')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => array(
      0 => array(
        'value' => '1',
      ),
    ),
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 1,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '1',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_status',
    'label' => $t('Status'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => '-3',
    ),
  );
  if (!field_info_instance('node', 'field_issue_status', 'project_issue')) {
    field_create_instance($instance);
  }

  // Priority
  $field = array(
    'active' => '1',
    'cardinality' => '1',
    'deleted' => '0',
    'entity_types' => array(),
    'field_name' => 'field_issue_priority',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        400 => $t('Critical'),
        300 => $t('Major'),
        200 => $t('Normal'),
        100 => $t('Minor'),
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => '0',
    'type' => 'list_integer',
  );
  if (!field_info_field('field_issue_priority')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => array(
      0 => array(
        'value' => '200',
      ),
    ),
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 2,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 2,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '2',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_priority',
    'label' => $t('Priority'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => '-3',
    ),
  );
  if (!field_info_instance('node', 'field_issue_priority', 'project_issue')) {
    field_create_instance($instance);
  }

  // Category
  $field = array(
    'cardinality' => '1',
    'entity_types' => array(),
    'field_name' => 'field_issue_category',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(
        1 => $t('Bug report'),
        2 => $t('Task'),
        3 => $t('Feature request'),
        4 => $t('Support request'),
      ),
      'allowed_values_function' => '',
    ),
    'translatable' => '0',
    'type' => 'list_integer',
  );
  if (!field_info_field('field_issue_category')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => NULL,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 3,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 3,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '3',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_category',
    'label' => $t('Category'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => '-3',
    ),
  );
  if (!field_info_instance('node', 'field_issue_category', 'project_issue')) {
    field_create_instance($instance);
  }

  // Component
  $field = array(
    'cardinality' => '1',
    'entity_types' => array(),
    'field_name' => 'field_issue_component',
    'foreign keys' => array(),
    'indexes' => array(
      'value' => array(
        0 => 'value',
      ),
    ),
    'module' => 'list',
    'settings' => array(
      'allowed_values' => array(),
      'allowed_values_function' => 'dereference_list_allowed_list_values',
      'entityreference_field' => 'field_project',
      'dereferenced_field' => 'field_project_components',
    ),
    'translatable' => '0',
    'type' => 'list_text',
  );
  if (!field_info_field('field_issue_component')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => NULL,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => 4,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 4,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'list',
        'settings' => array(),
        'type' => 'list_default',
        'weight' => '4',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_component',
    'label' => $t('Component'),
    'required' => 1,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => '-3',
    ),
  );
  if (!field_info_instance('node', 'field_issue_component', 'project_issue')) {
    field_create_instance($instance);
  }

  // Assigned
  $field = array(
    'cardinality' => '1',
    'entity_types' => array(),
    'field_name' => 'field_issue_assigned',
    'module' => 'entityreference',
    'settings' => array(
      'target_type' => 'user',
      'handler' => 'assigned',
      'handler_submit' => 'Change handler',
      'handler_settings' => array(
        'project_field' => 'field_project',
        'sort' => array(
          'type' => 'property',
          'property' => 'name',
          'field' => 'none',
          'direction' => 'ASC',
        ),
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
      ),
    ),
    'translatable' => '0',
    'type' => 'entityreference',
  );
  if (!field_info_field('field_issue_assigned')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'default_value' => NULL,
    'description' => '',
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => '5',
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 5,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => '5',
      ),
    ),
    'entity_type' => 'node',
    'field_name' => 'field_issue_assigned',
    'label' => $t('Assigned'),
    'required' => 0,
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'widget' => array(
      'active' => 1,
      'module' => 'options',
      'settings' => array(),
      'type' => 'options_select',
      'weight' => '-2',
      /// @todo: We need an allowed values callback to restrict the options.
    ),
  );
  if (!field_info_instance('node', 'field_issue_assigned', 'project_issue')) {
    field_create_instance($instance);
  }

  // Project
  $field = array(
    'entity_types' => array(),
    'settings' => array(
      'target_type' => 'node',
      'handler' => 'base',
      'handler_submit' => 'Change handler',
      'handler_settings' => array(
        'target_bundles' => array(
          'project' => 'project',
        ),
      ),
    ),
    'field_name' => 'field_project',
    'type' => 'entityreference',
    'module' => 'entityreference',
    'cardinality' => '1',
    'columns' => array(
      'target_id' => array(
        'description' => 'The id of the target entity',
        'type' => 'int',
        'unsigned' => TRUE,
        'not null' => FALSE,
      ),
    ),
  );
  if (!field_info_field('field_project')) {
    field_create_field($field);
  }

  $instance = array(
    'bundle' => 'project_issue',
    'entity_types' => array(),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'module' => 'entityreference',
        'settings' => array(),
        'type' => 'entityreference_label',
        'weight' => 0,
      ),
      'teaser' => array(
        'label' => 'above',
        'settings' => array(),
        'type' => 'hidden',
        'weight' => 0,
      ),
      'issuemetadata' => array(
        'label' => 'inline',
        'module' => 'entityreference',
        'settings' => array(
          'link' => FALSE,
        ),
        'type' => 'entityreference_label',
        'weight' => '0',
      ),
    ),
    'settings' => array(
      'target_type' => 'node',
      'handler' => 'base',
      'handler_submit' => 'Change handler',
      'handler_settings' => array(
        'target_bundles' => array(
          'project' => 'project',
        ),
        'sort' => array(
          'type' => 'none',
          'property' => 'nid',
          'field' => 'body:value',
          'direction' => 'ASC',
        ),
        'behaviors' => array(
          'views-select-list' => array(
            'status' => 0,
          ),
        ),
      ),
    ),
    'entity_type' => 'node',
    'required' => 1,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_project',
    'label' => $t('Project'),
    'type' => 'entityreference',
    'module' => 'entityreference',
    'widget' => array(
      'weight' => -1,
      'type' => 'entityreference_autocomplete',
      'active' => 1,
      'settings' => array(
        'match_operator' => 'CONTAINS',
        'size' => '60',
        'path' => '',
      ),
    ),
  );
  if (!field_info_instance('node', 'field_project', 'project_issue')) {
    field_create_instance($instance);
  }

  // File field
  $field = array(
    'field_name' => 'field_issue_files',
    'cardinality' => FIELD_CARDINALITY_UNLIMITED,
    'type' => 'file',
    'settings' => array(
      'display_field' => 0,
      'display_default' => 0,
      'uri_scheme' => 'public',
    ),
    'module' => 'file',
    'active' => '1',
    'locked' => '0',
  );
  if (!field_info_field('field_issue_files')) {
    field_create_field($field);
  }

  $instance = array(
    'field_name' => 'field_issue_files',
    'entity_type' => 'node',
    'bundle' => 'project_issue',
    'label' => $t('Files'),
    'widget' => array(
      'weight' => 0,
      'type' => 'file_generic',
      'module' => 'file',
      'active' => 1,
      'settings' => array(
        'progress_indicator' => 'throbber',
      ),
    ),
    'settings' => array(
      'file_directory' => 'issues',
      'file_extensions' => 'jpg jpeg gif png txt xls pdf ppt pps odt ods odp gz tgz patch diff zip test info po pot psd',
      'max_filesize' => '',
      'description_field' => 1,
      'user_register_form' => false,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'file_default',
        'settings' => array(
        ),
        'module' => 'file',
        'weight' => 4,
      ),
      'teaser' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(
        ),
        'weight' => 0,
      ),
      'issuemetadata' => array(
        'type' => 'hidden',
        'label' => 'above',
        'settings' => array(
        ),
        'weight' => 0,
      ),
    ),
    'required' => 0,
    'description' => '',
  );
  if (!field_info_instance('node', 'field_issue_files', 'project_issue')) {
    field_create_instance($instance);
  }

  // Issue changes.
  $field = array(
    'translatable' => '0',
    'settings' => array(
      'properties' => array(
        'title' => 'title',
      ),
    ),
    'field_name' => 'field_issue_changes',
    'type' => 'nodechanges_revision_diff',
    'cardinality' => FIELD_CARDINALITY_UNLIMITED,
  );
  if (!field_info_field('field_issue_changes')) {
    field_create_field($field);
  }

  $instance = array(
    'label' => $t('Issue changes'),
    'widget' => array(
      'weight' => '1',
      'type' => 'field_extrawidgets_hidden',
      'active' => 1,
      'settings' => array(
        'formatter' => 'nodechanges_diff_default',
        'display_empty' => 0,
      ),
    ),
    'settings' => array(
      'user_register_form' => FALSE,
    ),
    'display' => array(
      'default' => array(
        'label' => 'above',
        'type' => 'nodechanges_diff_default',
        'settings' => array(),
        'weight' => 1,
      ),
      'teaser' => array(
        'label' => 'above',
        'type' => 'hidden',
        'settings' => array(),
        'weight' => 1,
      ),
      'issuemetadata' => array(
        'label' => 'above',
        'type' => 'hidden',
        'settings' => array(),
        'weight' => 1,
      ),
    ),
    'required' => 1,
    'description' => '',
    'default_value' => NULL,
    'field_name' => 'field_issue_changes',
    'entity_type' => 'comment',
    'bundle' => 'comment_node_project_issue',
  );
  if (!field_info_instance('comment', 'field_issue_changes', 'project_issue')) {
    field_create_instance($instance);
  }
}

/**
 * Uninstall the default project issue node type.
 */
function _project_issue_delete_issue_node_type() {
  $fields = array(
    'field_issue_status',
    'field_issue_priority',
    'field_issue_category',
    'field_issue_component',
    'field_issue_assigned',
    'field_project',
    'field_issue_changes',
  );
  foreach ($fields as $field) {
    field_delete_field($field);
    watchdog('project', 'Deleted the :field_name field from all content type instances.', array(':field_name' => $field));
  }
}
