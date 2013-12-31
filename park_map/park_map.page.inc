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
  $query = db_select('node', 'n');
  $query->condition('status', 1, '=');
  $query->condition('n.language', $value['lang'], '=');

  //quitamos el elemento de idioma para recorrer los fields.
  unset($value['lang']);

  foreach ($value as $field => $value) {
    $fieldtable = 'field_data_' . $field;
    $fieldvalue = $field . '_value';
    $fieldrelation = $field . ' .entity_id = node.nid';
    $fieldcondition = $field . '.' . $field_value;
    $query->leftJoin($fieldtable, $field, $fieldrelation);

    $query->condition($fieldcondition, $value, '=');
  }


/*
select node.title, node.nid from node node

inner join field_data_field_listado_de_elementos field_data_field_listado_de_elementos 
  ON (field_data_field_listado_de_elementos.entity_id = node.nid)

where node.language = 'en' 
and node.status = 1 


and field_data_field_listado_de_elementos.field_listado_de_elementos_value IN (1,2,3)

*/

    /*
   $db_or = db_or(); 
   foreach ($value as $field => $elements) {
    $db_or->condition('n.type', $value, '=');
   }
  
   $query->condition($db_or);
   */
   $data = 'god response';

    
    
//print drupal_json_output (

/*
  $response = array(
    'status' => 1,
    'data' => $data,
  );
   */
  return 'hola mundo';
}

/**
 *
 */
function query_park_map_delivery_callback($page_callback_result) {
  print  $page_callback_result;
  ajax_footer();
}

////// Funtions to improve the query.

/**
 *
 */