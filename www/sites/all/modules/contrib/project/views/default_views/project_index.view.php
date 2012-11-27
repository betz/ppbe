<?php

/**
 * @file
 * An index of all projects of a certain project type by title.
 */
$view = new view;
$view->name = 'project_index';
$view->description = 'Listing of all projects by title';
$view->tag = '';
$view->base_table = 'node';
$view->human_name = '';
$view->core = 0;
$view->api_version = '3.0';
$view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

/* Display: Defaults */
$handler = $view->new_display('default', 'Defaults', 'default');
$handler->display->display_options['access']['type'] = 'none';
$handler->display->display_options['cache']['type'] = 'none';
$handler->display->display_options['query']['type'] = 'views_query';
$handler->display->display_options['query']['options']['distinct'] = TRUE;
$handler->display->display_options['query']['options']['query_comment'] = FALSE;
$handler->display->display_options['exposed_form']['type'] = 'basic';
$handler->display->display_options['pager']['type'] = 'none';
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['style_options']['grouping'] = '';
$handler->display->display_options['style_options']['type'] = 'ol';
$handler->display->display_options['row_plugin'] = 'fields';
/* Field: Content: Title */
$handler->display->display_options['fields']['title']['id'] = 'title';
$handler->display->display_options['fields']['title']['table'] = 'node';
$handler->display->display_options['fields']['title']['field'] = 'title';
$handler->display->display_options['fields']['title']['label'] = '';
$handler->display->display_options['fields']['title']['alter']['alter_text'] = 0;
$handler->display->display_options['fields']['title']['alter']['make_link'] = 0;
$handler->display->display_options['fields']['title']['alter']['word_boundary'] = 1;
$handler->display->display_options['fields']['title']['alter']['ellipsis'] = 1;
$handler->display->display_options['fields']['title']['alter']['strip_tags'] = 0;
$handler->display->display_options['fields']['title']['alter']['trim'] = 0;
$handler->display->display_options['fields']['title']['alter']['html'] = 0;
$handler->display->display_options['fields']['title']['hide_empty'] = 0;
$handler->display->display_options['fields']['title']['empty_zero'] = 0;
$handler->display->display_options['fields']['title']['link_to_node'] = 1;
/* Sort criterion: Content: Title */
$handler->display->display_options['sorts']['title']['id'] = 'title';
$handler->display->display_options['sorts']['title']['table'] = 'node';
$handler->display->display_options['sorts']['title']['field'] = 'title';
/* Contextual filter: Project node type */
$handler->display->display_options['arguments']['type']['id'] = 'type';
$handler->display->display_options['arguments']['type']['table'] = 'node';
$handler->display->display_options['arguments']['type']['field'] = 'type';
$handler->display->display_options['arguments']['type']['ui_name'] = 'Project node type';
$handler->display->display_options['arguments']['type']['default_action'] = 'not found';
$handler->display->display_options['arguments']['type']['title_enable'] = 1;
$handler->display->display_options['arguments']['type']['title'] = '%1 index';
$handler->display->display_options['arguments']['type']['default_argument_type'] = 'fixed';
$handler->display->display_options['arguments']['type']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['type']['specify_validation'] = 1;
$handler->display->display_options['arguments']['type']['validate']['type'] = 'project_node_type';
/* Filter criterion: Content: Published */
$handler->display->display_options['filters']['status']['id'] = 'status';
$handler->display->display_options['filters']['status']['table'] = 'node';
$handler->display->display_options['filters']['status']['field'] = 'status';
$handler->display->display_options['filters']['status']['value'] = '1';
$handler->display->display_options['filters']['status']['group'] = '0';
$handler->display->display_options['filters']['status']['expose']['operator'] = FALSE;
/* Filter criterion: Project type */
$handler->display->display_options['filters']['field_project_type_value']['id'] = 'field_project_type_value';
$handler->display->display_options['filters']['field_project_type_value']['table'] = 'field_data_field_project_type';
$handler->display->display_options['filters']['field_project_type_value']['field'] = 'field_project_type_value';
$handler->display->display_options['filters']['field_project_type_value']['ui_name'] = 'Project type';
$handler->display->display_options['filters']['field_project_type_value']['value'] = array(
  'full' => 'full',
);
$handler->display->display_options['filters']['field_project_type_value']['exposed'] = TRUE;
$handler->display->display_options['filters']['field_project_type_value']['expose']['operator_id'] = 'field_project_type_value_op';
$handler->display->display_options['filters']['field_project_type_value']['expose']['label'] = 'Project type';
$handler->display->display_options['filters']['field_project_type_value']['expose']['operator'] = 'field_project_type_value_op';
$handler->display->display_options['filters']['field_project_type_value']['expose']['identifier'] = 'project-status';
$handler->display->display_options['filters']['field_project_type_value']['expose']['reduce'] = 0;

/* Display: Page */
$handler = $view->new_display('page', 'Page', 'page_1');
$handler->display->display_options['path'] = 'project/%/index';

/* Display: Block */
$handler = $view->new_display('block', 'Block', 'block_1');
$handler->display->display_options['defaults']['use_more'] = FALSE;
$handler->display->display_options['use_more'] = TRUE;
$handler->display->display_options['use_more_always'] = TRUE;
$handler->display->display_options['use_more_text'] = 'View full index';
$handler->display->display_options['defaults']['style_plugin'] = FALSE;
$handler->display->display_options['style_plugin'] = 'list';
$handler->display->display_options['style_options']['grouping'] = '';
$handler->display->display_options['defaults']['style_options'] = FALSE;
$handler->display->display_options['defaults']['row_plugin'] = FALSE;
$handler->display->display_options['row_plugin'] = 'fields';
$handler->display->display_options['defaults']['row_options'] = FALSE;
$handler->display->display_options['defaults']['arguments'] = FALSE;
/* Contextual filter: Project Node Type */
$handler->display->display_options['arguments']['type']['id'] = 'type';
$handler->display->display_options['arguments']['type']['table'] = 'node';
$handler->display->display_options['arguments']['type']['field'] = 'type';
$handler->display->display_options['arguments']['type']['ui_name'] = 'Project Node Type';
$handler->display->display_options['arguments']['type']['default_action'] = 'default';
$handler->display->display_options['arguments']['type']['title_enable'] = 1;
$handler->display->display_options['arguments']['type']['title'] = '%1 index';
$handler->display->display_options['arguments']['type']['default_argument_type'] = 'project_plugin_argument_default_project_type';
$handler->display->display_options['arguments']['type']['default_argument_skip_url'] = 0;
$handler->display->display_options['arguments']['type']['summary']['number_of_records'] = '0';
$handler->display->display_options['arguments']['type']['summary']['format'] = 'default_summary';
$handler->display->display_options['arguments']['type']['summary_options']['items_per_page'] = '25';
$handler->display->display_options['arguments']['type']['specify_validation'] = 1;
$handler->display->display_options['arguments']['type']['validate']['type'] = 'project_node_type';
