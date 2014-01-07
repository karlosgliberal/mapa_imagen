<?php


/**
 * @file
 */

/**
 *
 */
function print_park_map() {

  //$values = new stdClass;
  $values = new stdClass;
  
  // get the image path
  $image_absolute_path = _park_map_load_image_map_url();
  
  if(isset($image_absolute_path)){
    $values->image_absolute_path =  $image_absolute_path;
  }
  
  // get the geolocation values:
  $park_map_load_map_geolocation = _park_map_load_map_geolocation();

  if(isset($park_map_load_map_geolocation)){
    $values->geolocation  =  $park_map_load_map_geolocation;
  }

  // get the firs load.
  $park_map_first_load_data = _park_map_first_load_data();
  
  if(isset($park_map_first_load_data)){
    $values->first_load = $park_map_first_load_data;
  }

  // get all the cotent types and all the fields from every content type.
  $park_map_load_content_options = _park_map_load_content_options();

  if(isset($park_map_load_content_options)){
    $values->contents =  $park_map_load_content_options;
  }

  drupal_add_js( array('park_map' => $values), 'setting');

  return theme('render_park_map', array ($values));
}


/**
 *
 */
function query_park_map() {

  $value = _park_map_url_parser($_SERVER ['QUERY_STRING']);


  if(!is_array($value) ||  !$value['lang']) {

    $response = array(
      'status' => 400,
      'data' => 'Bad Request',
    );  

    return $response;
  }

  // montamos la query. básica
  $query = db_select('node', 'node');
  $query->condition('status', 1, '=');
  $query->condition('node.language', $value['lang'], '=');
  $query->fields('node', array('title','nid'));

  //imagen field_data_field_image
  $query->leftJoin('field_data_field_image', 'field_image','field_image.entity_id = node.nid');
  $query->fields('field_image', array('field_image_fid'));


  //quitamos el elemento de idioma para recorrer los fields.
  unset($value['lang']);

  foreach ($value as $field => $value) {

    $infofield = field_info_field($field);

    if($infofield['type'] = 'taxonomy_term_type'){
      $fieldvalue = $field . '_tid';  
    }else {
      $fieldvalue = $field . '_value';
    }

    $fieldtable = 'field_data_' . $field;
    $fieldrelation = $field . ' .entity_id = node.nid';
    $fieldcondition = $field . '.' . $fieldvalue;
    $query->leftJoin($fieldtable, $field, $fieldrelation);

    $query->condition($fieldcondition, $value, '=');
  }

  $result = $query->execute();

  
  
  //degug
  dpq($query); # using devel
  print_r($query->__toString());
  

  $response = array();
  $response['status'] = 200;
  foreach ($result as $key => $record) {
    


    $response['data'][$key]['nid'] = $record->nid;
    $response['data'][$key]['title'] = $record->title;
    $response['data'][$key]['image'] = _park_map_get_image_path($record->field_image_fid,'thumbnail');
    }

dpm($response);

  return 'hola mundo';// $response;
}

/**
 *
 */
function query_park_map_delivery_callback($page_callback_result) {
 drupal_json_output ( $page_callback_result);
  ajax_footer();
}

////// Funtions to improve the query.

/**
 *
 */
