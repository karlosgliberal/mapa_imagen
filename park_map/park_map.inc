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
  *  Null si no hay nada seleccoinado o un Array con los elementos.
  */

function _park_map_bundles_selected ($values) {

  if(!is_array($values)) {
    return NULL;
  }
  
  $type = array();

  foreach ($values as $value) {
    if($value){
      $type[] =$value;
    }
  }
  return $type;
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
      watchdog(WATCHDOG_PARK_MAP_TYPE, 'park_map_southwest_latlong variable is empty', $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL);
    }

    $geolocation->northeast_latlong = variable_get_value('park_map_northeast_latlong');
    if(!$geolocation->northeast_latlong){
      watchdog(WATCHDOG_PARK_MAP_TYPE, 'park_map_northeast_latlong variable is empty', $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL);
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

  $contents = cache_get('park-map:options');

  if(!$contents){
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

    cache_set('park-map:options', $content);

  }else {
    $content = $contents->data;
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

//  return NULL;

}

/**
 * @name
 * _park_map_first_load_data()
 * @return
 * NULL if there is nothing selected, an array of elements if there are something selected.
 */
function _park_map_first_load_data() {

  global $language;

  $contents = cache_get('park-map-' . $language->language . ':firsload');

  $contents = null;

 if(!$contents){

    $types = variable_get_value('park_map_first_load_bundles');

    $quantity = variable_get_value('park_map_first_load_quantity');

    if (!is_array($types) || !$types || !is_numeric($quantity) ) {
      watchdog(WATCHDOG_PARK_MAP_TYPE, 'Park map first load is not ok, please check de the form', $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL);

      return NULL;
    }

    $query = db_select('node', 'node')
      ->fields('node', array('nid'))
      ->condition('language', $language->language, '=')
      ->condition('type', $types, 'IN')
      ->range(0, $quantity);
      $records = $query->execute();
      
      $result = array();
      foreach ($records as $record) {
        $result [] = $record->nid;
      }
 
     $entitys = node_load_multiple($result); 
      
 
    if($entitys) {

      cache_set('park-map-' . $language->language . ':firsload', $entitys);

      return $entitys;

    } else {
       watchdog(WATCHDOG_PARK_MAP_TYPE, 'Park map first load is not ok, the query give back nothing.', $variables = array(), $severity = WATCHDOG_NOTICE, $link = NULL);      

       return NULL;
     }
     
   
  }    else {

    return $contents->data;
  }
}