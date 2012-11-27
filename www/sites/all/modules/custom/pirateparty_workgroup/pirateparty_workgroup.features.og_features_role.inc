<?php
/**
 * @file
 * pirateparty_workgroup.features.og_features_role.inc
 */

/**
 * Implements hook_og_features_default_roles().
 */
function pirateparty_workgroup_og_features_default_roles() {
  $roles = array();

  // Exported OG Role: 'node:workgroup:content editor'.
  $roles['node:workgroup:content editor'] = array(
    'gid' => 0,
    'group_type' => 'node',
    'group_bundle' => 'workgroup',
    'name' => 'content editor',
  );

  return $roles;
}
