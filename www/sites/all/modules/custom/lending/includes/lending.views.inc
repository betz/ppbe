<?php // -*- drupal -*-

  /**
   * @file
   *
   * Provide views access into the checkouts data
   *
   */

  /**
   * Implements hook_views_data().
   */
function lending_views_data() {
  $data;
  _lending_define_checkouts(&$data);
  _lending_define_requests(&$data);

  return $data;
}

function _lending_define_checkouts(&$data) {

  $data['lending_checkouts']['table']['group'] = t('Lending Checkouts');

  /* // Define this as a base table. In reality this is not very useful for */
  /* // this table, as it isn't really a distinct object of its own, but */
  /* // it makes a good example. */
  /* $data['lending_checkouts']['table']['base'] = array( */
  /*   'field' => 'cid', */
  /*   'title' => t('Lending Checkouts table'), */
  /*   'help' => t("Checked out items"), */
  /*   'weight' => -12 */
  /* ); */

  $data['lending_checkouts']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'created_lid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'returned_lid',
    ),
  );

  // Next, describe each of the individual fields in this table to Views. For
  // each field, you may define what field, sort, argument, and/or filter
  // handlers it supports. This will determine where in the Views interface you
  // may use the field.

  // Node ID field.
  _lending_define_view_number($data, 'lending_checkouts', 'cid', 'Checkout id', 'Unique id associated with the checkout');

  _lending_define_view_date($data, 'lending_checkouts', 'created_at', 'Checked out at', 'When the item was checked out');

  _lending_define_view_date($data, 'lending_checkouts', 'returned_at', 'Returned at', 'When the item was checked back in');


  _lending_define_view_reference(
    $data,
    'lending_checkouts',
    'nid',
    'Node Id',
    'Node that was checked out',
    'node',
    'nid'
  );

  _lending_define_view_reference(
    $data,
    'lending_checkouts',
    'uid',
    'User Id',
    'User that checked out this item',
    'users',
    'uid'
  );

  _lending_define_view_reference(
    $data,
    'lending_checkouts',
    'created_lid',
    'Librarian Id (Checkout)',
    'Librarian that checked out this item',
    'users',
    'uid'
  );

  _lending_define_view_reference(
    $data,
    'lending_checkouts',
    'returned_lid',
    'Librarian Id (Checkin) ',
    'Librarian that checked in this item',
    'users',
    'uid'
  );

}

function _lending_define_requests(&$data) {

  $data['lending_requests']['table']['group'] = t('Lending Requests');

  /* // Define this as a base table. In reality this is not very useful for */
  /* // this table, as it isn't really a distinct object of its own, but */
  /* // it makes a good example. */
  /* $data['lending_requests']['table']['base'] = array( */
  /*   'field' => 'rid', */
  /*   'title' => t('Lending Requests table'), */
  /*   'help' => t("Requests to lend out an item, user initiated"), */
  /*   'weight' => -12 */
  /* ); */

  $data['lending_requests']['table']['join'] = array(
    // Index this array by the table name to which this table refers.
    // 'left_field' is the primary key in the referenced table.
    // 'field' is the foreign key in this table.
    'node' => array(
      'left_field' => 'nid',
      'field' => 'nid',
    ),
    'users' => array(
      'left_field' => 'uid',
      'field' => 'uid',
    ),
  );

  // Next, describe each of the individual fields in this table to Views. For
  // each field, you may define what field, sort, argument, and/or filter
  // handlers it supports. This will determine where in the Views interface you
  // may use the field.

  // Node ID field.
  _lending_define_view_number($data, 'lending_requests', 'rid', 'Request id', 'Unique id associated with the request');

  _lending_define_view_date($data, 'lending_requests', 'created_at', 'Requested at', 'When the item was requested');

  _lending_define_view_date($data, 'lending_requests', 'deleted_at', 'Deleted at', 'When the request was removed, may be user or librarian initiated');

  _lending_define_view_reference(
    $data,
    'lending_requests',
    'nid',
    'Node Id',
    'Node that was requested',
    'node',
    'nid'
  );

  _lending_define_view_reference(
    $data,
    'lending_requests',
    'uid',
    'User Id',
    'User that requested this item',
    'users',
    'uid'
  );

}



function _lending_define_view_date(&$data, $table, $column, $title, $help = '') {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_date',
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_string_compare'),
    // Amazon date is sortable, so we can just use the standard handler.
    'sort' => array('handler' => 'views_handler_sort'),
  );
}

function _lending_define_view_reference(&$data, $table, $column, $title, $help, $base, $field) {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_numeric',
    ),
    // Information for displaying a title as a field

    'relationship' => array(
      'base' => $base,
      'field' => $field,
      'handler' => 'views_handler_relationship',
      'label' => $title,
    ),
  );

}

/**
 * utility function borrowed from the amazon module
 */
function _lending_define_view_number(&$data, $table, $column, $title, $help) {
  $data[$table][$column] = array(
    'title' => $title, // The item it appears as on the UI,
    'help' => $help, // The help that appears on the UI,
    // Information for displaying a title as a field
    'field' => array(
      'field' => $column, // the real field
      'handler' => 'views_handler_field_numeric',
      'click sortable' => TRUE,
    ),
    'filter' => array('handler' => 'views_handler_filter_numeric'),
    'sort' => array('handler' => 'views_handler_sort'),
  );
}


/***********************************************************************
 *
 * Implements hook_views_default_views().
 *
 *  Base views.  These are huge, so I hope I can get them into another file.
 *
 *  These are built in the qeury building and exported out with minor tweaks.
 */

function lending_views_default_views() {
  $view = new view;
  $view->name = 'lending_checkedout_items';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
      'uid' => array(
        'label' => 'User Id',
        'required' => 1,
        'id' => 'uid',
        'table' => 'lending_checkouts',
        'field' => 'uid',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('fields', array(
      'title' => array(
        'label' => 'Title',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 1,
        'exclude' => 0,
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'relationship' => 'none',
        'override' => array(
          'button' => 'Override',
        ),
      ),
      'type' => array(
        'label' => 'Type',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'machine_name' => 0,
        'exclude' => 0,
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'relationship' => 'none',
      ),
      'cid' => array(
        'label' => 'Checkout Id',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'set_precision' => FALSE,
        'precision' => 0,
        'decimal' => '.',
        'separator' => ',',
        'prefix' => '',
        'suffix' => '',
        'exclude' => 1,
        'id' => 'cid',
        'table' => 'lending_checkouts',
        'field' => 'cid',
        'relationship' => 'none',
      ),
      'value' => array(
        'label' => 'Current Lendee',
        'alter' => array(
          'alter_text' => 0,
          'text' => ' ( [value] )',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_user' => 1,
        'exclude' => 0,
        'id' => 'value',
        'table' => 'profile_values_profile_fullname',
        'field' => 'value',
        'relationship' => 'uid',
      ),
      'name' => array(
        'label' => 'User Id',
        'alter' => array(
          'alter_text' => 1,
          'text' => ' ( [name] )',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_user' => 1,
        'overwrite_anonymous' => 0,
        'anonymous_text' => '',
        'exclude' => 0,
        'id' => 'name',
        'table' => 'users',
        'field' => 'name',
        'relationship' => 'uid',
      ),
      'created_at' => array(
        'label' => 'Checked out on',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'date_format' => 'custom',
        'custom_date_format' => 'M jS, Y',
        'exclude' => 0,
        'id' => 'created_at',
        'table' => 'lending_checkouts',
        'field' => 'created_at',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('sorts', array(
      'title' => array(
        'order' => 'ASC',
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('filters', array(
      'returned_at' => array(
        'operator' => '=',
        'value' => '0',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'case' => 1,
        'id' => 'returned_at',
        'table' => 'lending_checkouts',
        'field' => 'returned_at',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => 'access lending library',
    ));
  $handler->override_option('cache', array(
      'type' => 'none',
    ));
  $handler->override_option('header', '<h1>Currently Checked out Items</h1>');
  $handler->override_option('header_format', '2');
  $handler->override_option('header_empty', 0);
  $handler->override_option('items_per_page', 30);
  $handler->override_option('use_pager', '1');
  $handler->override_option('distinct', 0);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'asc',
      'columns' => array(
        'title' => 'title',
        'type' => 'type',
        'cid' => 'cid',
        'value' => 'value',
        'name' => 'value',
        'created_at' => 'created_at',
      ),
      'info' => array(
        'title' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'type' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'cid' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'name' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'created_at' => array(
          'sortable' => 0,
          'separator' => '',
        ),
      ),
      'default' => '-1',
    ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'lending/checkouts');
  $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
  $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));

  // Add view to list of views to provide.
  $views[$view->name] = $view;

  // The requests view
  $view = new view;
  $view->name = 'lending_requests';
  $view->description = '';
  $view->tag = '';
  $view->view_php = '';
  $view->base_table = 'node';
  $view->is_cacheable = FALSE;
  $view->api_version = 2;
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */
  $handler = $view->new_display('default', 'Defaults', 'default');
  $handler->override_option('relationships', array(
      'uid' => array(
        'id' => 'uid',
        'table' => 'lending_requests',
        'field' => 'uid',
      ),
    ));
  $handler->override_option('fields', array(
      'title' => array(
        'label' => 'Title',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 1,
        'exclude' => 0,
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'relationship' => 'none',
        'override' => array(
          'button' => 'Override',
        ),
      ),
      'type' => array(
        'label' => 'Type',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_node' => 0,
        'machine_name' => 0,
        'exclude' => 0,
        'id' => 'type',
        'table' => 'node',
        'field' => 'type',
        'relationship' => 'none',
      ),
      'value' => array(
        'label' => 'Requested by',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_user' => 1,
        'exclude' => 0,
        'id' => 'value',
        'table' => 'profile_values_profile_fullname',
        'field' => 'value',
        'relationship' => 'uid',
      ),
      'name' => array(
        'label' => 'Name',
        'alter' => array(
          'alter_text' => 1,
          'text' => ' ( [name] )',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'link_to_user' => 1,
        'overwrite_anonymous' => 0,
        'anonymous_text' => '',
        'exclude' => 0,
        'id' => 'name',
        'table' => 'users',
        'field' => 'name',
        'relationship' => 'uid',
      ),
      'rid' => array(
        'label' => 'Request id',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'set_precision' => FALSE,
        'precision' => 0,
        'decimal' => '.',
        'separator' => ',',
        'prefix' => '',
        'suffix' => '',
        'exclude' => 1,
        'id' => 'rid',
        'table' => 'lending_requests',
        'field' => 'rid',
        'relationship' => 'none',
      ),
      'created_at' => array(
        'label' => 'Requested on',
        'alter' => array(
          'alter_text' => 0,
          'text' => '',
          'make_link' => 0,
          'path' => '',
          'link_class' => '',
          'alt' => '',
          'prefix' => '',
          'suffix' => '',
          'target' => '',
          'help' => '',
          'trim' => 0,
          'max_length' => '',
          'word_boundary' => 1,
          'ellipsis' => 1,
          'html' => 0,
          'strip_tags' => 0,
        ),
        'empty' => '',
        'hide_empty' => 0,
        'empty_zero' => 0,
        'date_format' => 'custom',
        'custom_date_format' => 'M jS, Y',
        'exclude' => 0,
        'id' => 'created_at',
        'table' => 'lending_requests',
        'field' => 'created_at',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('sorts', array(
      'title' => array(
        'order' => 'ASC',
        'id' => 'title',
        'table' => 'node',
        'field' => 'title',
        'relationship' => 'none',
      ),
      'created_at' => array(
        'order' => 'ASC',
        'id' => 'created_at',
        'table' => 'lending_requests',
        'field' => 'created_at',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('filters', array(
      'deleted_at' => array(
        'operator' => '=',
        'value' => '0',
        'group' => '0',
        'exposed' => FALSE,
        'expose' => array(
          'operator' => FALSE,
          'label' => '',
        ),
        'case' => 1,
        'id' => 'deleted_at',
        'table' => 'lending_requests',
        'field' => 'deleted_at',
        'relationship' => 'none',
      ),
    ));
  $handler->override_option('access', array(
      'type' => 'perm',
      'perm' => 'access lending library',
    ));
  $handler->override_option('cache', array(
      'type' => 'none',
    ));
  $handler->override_option('header', '<h1>Currently Requested Items</h1>');
  $handler->override_option('header_format', '2');
  $handler->override_option('header_empty', 0);
  $handler->override_option('items_per_page', 30);
  $handler->override_option('use_pager', '1');
  $handler->override_option('distinct', 0);
  $handler->override_option('style_plugin', 'table');
  $handler->override_option('style_options', array(
      'grouping' => '',
      'override' => 1,
      'sticky' => 0,
      'order' => 'asc',
      'columns' => array(
        'title' => 'title',
        'type' => 'type',
        'cid' => 'cid',
        'value' => 'value',
        'name' => 'value',
        'created_at' => 'created_at',
      ),
      'info' => array(
        'title' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'type' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'cid' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'value' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'name' => array(
          'sortable' => 0,
          'separator' => '',
        ),
        'created_at' => array(
          'sortable' => 0,
          'separator' => '',
        ),
      ),
      'default' => '-1',
    ));
  $handler = $view->new_display('page', 'Page', 'page_1');
  $handler->override_option('path', 'lending/requests');
  $handler->override_option('menu', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));
  $handler->override_option('tab_options', array(
      'type' => 'none',
      'title' => '',
      'description' => '',
      'weight' => 0,
      'name' => 'navigation',
    ));

  $views[$view->name] = $view;

  // ...Repeat all of the above for each view the module should provide.

  // At the end, return array of default views.
  return $views;
}