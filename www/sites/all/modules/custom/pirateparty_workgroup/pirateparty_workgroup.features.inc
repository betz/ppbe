<?php
/**
 * @file
 * pirateparty_workgroup.features.inc
 */

/**
 * Implements hook_ctools_plugin_api().
 */
function pirateparty_workgroup_ctools_plugin_api() {
  list($module, $api) = func_get_args();
  if ($module == "context" && $api == "context") {
    return array("version" => "3");
  }
  list($module, $api) = func_get_args();
  if ($module == "ds" && $api == "ds") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "page_manager" && $api == "pages_default") {
    return array("version" => "1");
  }
  list($module, $api) = func_get_args();
  if ($module == "strongarm" && $api == "strongarm") {
    return array("version" => "1");
  }
}

/**
 * Implements hook_views_api().
 */
function pirateparty_workgroup_views_api() {
  return array("version" => "3.0");
}

/**
 * Implements hook_node_info().
 */
function pirateparty_workgroup_node_info() {
  $items = array(
    'blog_post' => array(
      'name' => t('Blog Post'),
      'base' => 'node_content',
      'description' => t('Blog post, attached to a workgroup.'),
      'has_title' => '1',
      'title_label' => t('Title'),
      'help' => '',
    ),
    'event' => array(
      'name' => t('Event'),
      'base' => 'node_content',
      'description' => t('Event, attached to a workgroup.'),
      'has_title' => '1',
      'title_label' => t('Event Name'),
      'help' => '',
    ),
    'workgroup' => array(
      'name' => t('Workgroup'),
      'base' => 'node_content',
      'description' => t('A workgroup with members.'),
      'has_title' => '1',
      'title_label' => t('Workgroup Name'),
      'help' => '',
    ),
  );
  return $items;
}
