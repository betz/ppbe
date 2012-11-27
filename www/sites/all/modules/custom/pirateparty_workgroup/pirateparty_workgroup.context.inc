<?php
/**
 * @file
 * pirateparty_workgroup.context.inc
 */

/**
 * Implements hook_context_default_contexts().
 */
function pirateparty_workgroup_context_default_contexts() {
  $export = array();

  $context = new stdClass();
  $context->disabled = FALSE; /* Edit this to true to make a default context disabled initially */
  $context->api_version = 3;
  $context->name = 'workgroup';
  $context->description = '';
  $context->tag = '';
  $context->conditions = array(
    'node' => array(
      'values' => array(
        'workgroup' => 'workgroup',
      ),
      'options' => array(
        'node_form' => '0',
      ),
    ),
  );
  $context->reactions = array();
  $context->condition_mode = 1;
  $export['workgroup'] = $context;

  return $export;
}
