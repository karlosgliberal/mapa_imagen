<?php


/**
 * @file
 */

/**
 *
 */
function print_park_map() {

  $values = new stdClass;

  // get the image path
  $values->image_absolute_path = _park_map_load_image_map_url();
  
  // get the geolocation values:
  $values->geolocation  =  _park_map_load_map_geolocation();

  // get all the cotent types and all the fields from every content type.
  $values->contents =  _park_map_load_content_options();



  drupal_add_js( array('park_map' => $values), 'setting');

  return theme('render_park_map', array ($values));
}


/**
 *
 */
function query_park_map() {

$arguments = preg_split('/&/',  $_SERVER ['QUERY_STRING']);

foreach ($arguments as $argument) {
  $element = preg_split('/=/', $argument);
  $value[$element[0]] = $element[1];
 
}

print_r($value);

  try{

  if(!is_array($value)) {

    throw new Exception("Error Processing Request", 1);
    
  }else {
    // comprobamos que esta la variable de idioma y de tipos de contenido.sino esta no hacemos nada.
    if(!$value['lang']  || !$values['contenttype']) {

      $response = array(
        'status' => 400,
        'data' => 'Bad Request',
      );
      
      return $response;

    } else {
      // montamos la query.
      $query = db_select('node', 'n');
      $query->condition('status', 1, '=');
      $query->condition('n.language', $value['lang'], '=');

      $db_or = db_or(); 
      foreach ($contenttypes as $contenttype) {
        $db_or->condition('n.type', $value, '=');
      }
      $query->condition($db_or);


    }
    
  }
  


  } catch( Exception $e) {

    drupal_set_message(t('No se han recibido los valores bien en la petición'), 'error');

  }
//print drupal_json_output (

$data =  array (   'test'=>true,   'dummy'=>array(0,1),  'url' => $_SERVER['REQUEST_URI']);

  $response = array(
    'status' => 1,
    'data' => $data,
  );
 
  return drupal_json_output($response);
}

/**
 *
 */
function query_park_map_delivery_callback($page_callback_result) {
  print $page_callback_result;
  ajax_footer();
}

////// Funtions to improve the query.

/**
 *
 */