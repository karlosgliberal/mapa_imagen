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
  $image_cache = cache_get('park-map:image');

  if(!$image_cache) {

    $image  = file_load(variable_get_value('park_map_image'));
    if($image):
      $image_path = file_create_url($image->uri);
      $values->image_absolute_path = $image_path;
      cache_set('park-map:image', $image_path);
    endif; 
  } else {
    $values->image_absolute_path = $image_cache->data;
  }

  // get the latitude and logitudes values AKA geolocation
  // they are 4 values, 2 values for northEast and and another two for southWest
  $geolocation_cache = cache_get('park-map:positions');

  if(!$geolocation_cache) {

    $geolocation = new stdClass;
    $geolocation->southwet_llatlong = variable_get_value('park_map_southwest_latlong');
    $geolocation->northeast_latlong = variable_get_value('park_map_northeast_latlong');
    
    $values->geolocation = $geolocation;
    cache_set('park-map:geolocation', $geolocation);

  } else {
    $values->geolocation = $geolocation_cache->data;
  }

//  $settings = array(    'image_path' => $values->image_absolute_path,   );

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