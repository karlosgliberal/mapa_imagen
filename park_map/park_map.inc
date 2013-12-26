<?php

/**
 * @file
 * "Inc" file for not drupal functions.
 */


/**
 * @name
 *  _park_map_list_bundles
 *
 * @return
 *  an array with all the node bundles selectes to work with.
 */

function _park_map_list_bundles() {

  $bundles = field_info_bundles('node');

  foreach ($bundles as $key => $bundle) {
    $list_bundles[$key] = $bundle['label'];
  }

  return $list_bundles;
}


/**
  *@name 
  * _park_map_bundles_selected
  *
  * @var
  * $values is an array with diferente options. This value can from variable_get('park_map_bundle', NULL)
  *
  * @return
  * 0 is there is nothing or a number if there are bundles selected.
  */

function _park_map_bundles_selected ($values) {

  if(!is_array($values)){
    return NULL;
  }

  $a = 0;
  foreach ($values as $value) {

    $a += ($value)? 1 :  0 ;
  }

  return $a;
}


/**
 * @name
 *
 * @return 
 */
function _park_map_load_image_map_url() {

  // get the image path
  $image_cache = cache_get('park-map:image');

  if(!$image_cache) {

    $image  = file_load(variable_get_value('park_map_image'));

    if($image){

      $image_path = file_create_url($image->uri);
      cache_set('park-map:image', $image_path);
      return $image_path;
    }
  } else {
    return  $image_cache->data;
  }

}

/**
 * @name 
 *  _park_map_load_map_geolocation
 *
 * @return
 * An objet witth the geolocation values
 */
function _park_map_load_map_geolocation() {

  // get the latitude and logitudes values AKA geolocation
  // they are 4 values, 2 values for northEast and and another two for southWest
  $geolocation_cache = cache_get('park-map:positions');

  if(!$geolocation_cache) {

    $geolocation = new stdClass;

    $geolocation->southwet_latlong = variable_get_value('park_map_southwest_latlong');
    if(!$geolocation->southwet_latlong){
      watchdog('Park Map', 'park_map_southwest_latlong variable is empty', $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL);
    }

    $geolocation->northeast_latlong = variable_get_value('park_map_northeast_latlong');
    if(!$geolocation->northeast_latlong){
      watchdog('Park Map', 'park_map_northeast_latlong variable is empty', $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL);
    }

    cache_set('park-map:geolocation', $geolocation);
    return  $geolocation;

  } else {
    return $geolocation_cache->data;
  }
}

/**
 *
 */
function _park_map_load_content_options(){

// Get the list of bundles.
$bundles = variable_get_value('park_map_bundle');

if($bundles) {
  $nodes = field_info_bundles('node');
  $content = array();
  // Check the bundles to know how were selected.
  foreach ($bundles as $bundle) {

    // if the bundle is seleceted.
    if($bundle){

      // get the machine name
      $content[$bundle]['machinename'] = $bundle;
      // get the public name
      $content[$bundle]['publicname'] = $nodes[$bundle]['label'];

      // get the fields:
      $bundle_fields = variable_get_value('park_map_fields_' .  $bundle);

      foreach ($bundle_fields as $field) {
        
        if($field) {          

          $field_info = field_info_instance('node', $field, $bundle);
          $content[$bundle][$field]['name'] = $field;
          $content[$bundle][$field]['label'] = $field_info['label'];   
          $content[$bundle][$field]['options'] =  _park_map_get_field_options($field_info['widget']['type'], $field);
        }
      }
    }
  }
}

  return $content;
}



/**
 *
 */

function _park_map_get_field_options($widgetType, $fieldname) {

  switch ($widgetType) {
    case 'options_select':
      return _park_map_get_field_list_options($fieldname);
    break;

    case 'taxonomy_autocomplete':
      return _park_map_get_field_tag($fieldname);
    break;
    
  }

}




/**
 * @name _park_map_get_field_list_options
 * 
 * @var
 *  $fieldname the name of the field to get information
 *
 * @return 
 * an array with all the options or NULL if there are nothing to return.
 */
function _park_map_get_field_list_options($fieldname) {

  $field = field_info_field($fieldname);
    
  if($field['type'] == 'list_text'){
    return list_allowed_values($field);
  }

  if($field['type']== 'taxonomy_term_reference'){
    $vocab_machine_name = $field['settings']['allowed_values'][0]['vocabulary'];
    // Load the vocab by machine name
    $vocab = taxonomy_vocabulary_machine_name_load($vocab_machine_name);
        
    $terms = taxonomy_get_tree($vocab->vid);
    $options = array();
    foreach ($terms as $term) {
      $options[$term->tid] = $term->name;
    }

    return $options;
  }

  return NULL;
}


/**
 *
 */
function _park_map_get_field_tag($fieldname){

  $field = field_info_field($fieldname);
  //dpm($field);

  return NULL;




}