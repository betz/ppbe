<?php

class ProjectIssue_SelectionHandler_Assigned extends EntityReference_SelectionHandler_Generic implements EntityReference_SelectionHandler {
  public $project = NULL;

  /**
   * Factory function: create a new instance of this handler for a given field.
   *
   * @param $field
   *   A field datastructure.
   * @return EntityReferenceHandler
   */
  public static function getInstance($field, $instance = NULL, $entity_type = NULL, $entity = NULL) {
    $target_entity_type = $field['settings']['target_type'];

    // Check if the entity type does exist and has a base table.
    $entity_info = entity_get_info($target_entity_type);
    if (empty($entity_info['base table']) || $target_entity_type != 'user') {
      return EntityReference_SelectionHandler_Broken::getInstance($field, $instance);
    }

    return new ProjectIssue_SelectionHandler_Assigned($field, $instance, $entity_type, $entity);
  }

  /**
   * Generate a settings form for this handler.
   */
  public static function settingsForm($field, $instance) {
    $referenced_fields = array();
    foreach (field_info_fields() as $f) {
      if (isset($f['bundles'][$instance['entity_type']]) && in_array($instance['bundle'], $f['bundles'][$instance['entity_type']])  && $f['type'] == 'entityreference') {
        $referenced_fields[$f['field_name']] = $f['field_name'];
      }
    }

    $form['project_field'] = array(
      '#type' => 'select',
      '#title' => t('Field containing project reference'),
      '#options' => $referenced_fields,
      '#default_value' => isset($field['settings']['handler_settings']['project_field']) ? $field['settings']['handler_settings']['project_field'] : 'none',
    );

    $form += parent::settingsForm($field, $instance);

    return $form;
  }

  /**
   * Find all the users that are allowed to be assigned to the issue.
   *
   * @return array
   *   An array of users that can be assigned to the issue this selector is
   *   attached to. The array contains and is indexed by user ID (uid). If
   *   there are no allowed users or we hit some other error, the array will
   *   be empty.
   */
  protected function getAllowedAssignees() {
    $return = array();
    if ($this->entity) {
      $settings = $this->field['settings'];
      $project_field = field_info_field($settings['handler_settings']['project_field']);
      $project_field_items = field_get_items($this->entity_type, $this->entity, $project_field['field_name']);
      $referenced_project_id = $project_field_items[0]['target_id'];
      if ($referenced_project_id) {
        $projects = entity_load('node', array($referenced_project_id));
        $this->project = $projects[$referenced_project_id];
      }
      $project_field_items = field_get_items($this->entity_type, $this->entity, $this->field['field_name']);
      $current = $project_field_items[0]['target_id'];
      $return = project_issue_assigned_choices($this->entity, $this->project, $current);
    }
    return $return;
  }

  /**
   * Build an EntityFieldQuery to get referencable entities.
   */
  protected function buildEntityFieldQuery($match = NULL, $match_operator = 'CONTAINS') {
    $query = parent::buildEntityFieldQuery($match, $match_operator);

    // Search only from the list of users allowed to be assigned to this issue.
    $assignees = $this->getAllowedAssignees();
    if (!empty($assignees)) {
      $query->propertyCondition('uid', $assignees, 'IN');
    }

    return $query;
  }
}
