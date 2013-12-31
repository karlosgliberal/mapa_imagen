<?php




/**
 * @file
 *
 * this is the drupal module file, it will have all the required hooks.
 */

/**
* Implement Hook_menu().
*/
function park_map_menu() {

  $items = array();

  $items['admin/config/system/park_configuration'] = array(
    'title' => 'Park Map configuration',
    'description' => 'Select the elements to pass to js',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('variable_group_form', 'park_map_configuration'),
    'access arguments' => array('administer site configuration'),
  );

  $items['admin/config/system/park_configuration/configurations'] = array(
    'title' => 'Basic configuration',
    'description' => 'Select the elements to pass to js',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('variable_group_form', 'park_map_configuration'),
    'access arguments' => array('administer site configuration'),
    'type' => MENU_DEFAULT_LOCAL_TASK,
    'file' => 'park_map.router.inc',
  );

  $items['admin/config/system/park_configuration/options'] = array(
    'title' => 'Park Map configuration options',
    'description' => 'Select the elements to pass to js',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('variable_group_form', 'park_map_options'),
    'access arguments' => array('administer site configuration'),
    'type' => MENU_LOCAL_TASK    
  );  

  $items['admin/config/system/park_configuration/first_load'] = array(
    'title' => 'Park Map first load',
    'description' => 'Select the elements to load in the first visit',
    'page callback' => 'drupal_get_form',
    'page arguments' => array('variable_group_form', 'park_map_first_load'),
    'access arguments' => array('administer site configuration'),
    'type' => MENU_LOCAL_TASK    
  );  

  $items['park_map'] = array(
    'title' => 'Park Map',
    'page callback' => 'print_park_map',
    'access arguments' => array('access content'),
    'type' => MENU_NORMAL_ITEM,
  );

  $items['query/park_map'] = array(
    'title' => 'Ask for data',
    'page callback' => 'query_park_map',
    'access arguments' => array('access content'),
    'type' => MENU_CALLBACK,
    'file' => 'park_map.page.inc',
    'file path' => drupal_get_path('module','park_map'),
 //   'delivery callback' => 'query_park_map_delivery_callback' ,
    // https://gist.github.com/pascalduez/1517190
  );

  return $items;
}