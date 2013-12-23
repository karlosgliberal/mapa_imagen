<?php

/**
 *@file
 *
 * This file estore all the hooks required to create the field
 */

/**
 * Implement hook_field_info().
 *
 * Provides the description of the field
 *
 * 'default_widget' => 'field_coordinates_values', //this will be use in hook_field_widget_info
 * 'default_formatter' => 'coordinates_default',//this will be  use in hook_field_formater_info
 *
 */
function leaflet_map_field_field_info() {

  return array(
    'coordinates_data' => array(
      'label' => t('Leaflet field'),
      'description' => t('Add coordinates to leaflet interactive map'),
      'default_widget' => 'field_coordinates_values', 
      'default_formatter' => 'coordinates_default',
    ),
  );
}

/**
 * Implements hook_field_validate().
 */
function leaflet_map_field_field_validate($entity_type, $entity, $field, $instance, $langcode, $items, &$errors) {

  foreach ($items as $delta => $item) {
      
    if(!empty( $item['leaflet_coordinates'] ) ) {

      if(!is_string($item['leaflet_coordinates']) ) {
         $errors[$field['field_name']][$langcode][$delta][] = array(
           'error' => 'coordinates_data_invalid',
           'message' => t('Your data seems to be wrong.'),
         );       
      }
    }
  }
}

/**
 * Implements hook_field_is_empty().
 */
function leaflet_map_field_field_is_empty($item, $field) {

  if($field['type'] == 'coordinates_data') {
    if( empTy($item['leaflet_coordinates']) ) {
      return TRUE;
    }
  }

  return FALSE;
}

 /**
  * Implements hook_field_settings_form().
  * allow the module defining the field to add global settings
  * https://api.drupal.org/api/drupal/modules!field_ui!field_ui.api.php/function/hook_field_settings_form/7
  * 
  * Pseudo-hook.
  *
  * function interactive_map_field_field_settings_form($field, $instance, $has_data) {
  * Permite definir configuración de opciones del field en general, el mimos para todas las instancias del field
  * 
  * No es necesario en este proyecto.
  *
 */
/*
function interactive_map_field_field_settings_form ($field, $instance, $has_data) {

}
*/

/**
 * Implements hook_field_instance_settings_form
 * allow the module defining the field to add settings for a field instance.
 * http://api.drupal.org/api/drupal/modules!field_ui!field_ui.api.php/function/hook_field_instance_settings_form/7
 *
 * Pseudo-hook.
 *
 * function interactive_map_field_field_instance_settings_form($field, $instance){
 *
 * Permite definir configuraciones por cada instancia
 * http://drupal.stackexchange.com/questions/57653/difference-between-field-and-field-instance
 *
 * }
 */
/*
function  interactive_map_field_field_instance_settings_form($field, $instance) {

}
*/

///////////////// Widgets ///////////////////////

/**
 * Implements hook_field_widget_info().
 */
function leaflet_map_field_field_widget_info() {

  return array(
    'field_coordinates_values' => array(
      'label' => t('Leaflet Coordinates field'),
      'description' => t('Allow the user to enter the coordinates in the choose map'),
      'field types' => array('coordinates_data'),
      'behaviors' => array(
        'multiple values'  => FIELD_BEHAVIOR_DEFAULT,
        'default value' => FIELD_BEHAVIOR_DEFAULT,
      ),
    ),
  );
}

/**
 * Implements hook_field_settings_form().
 */
function leaflet_map_field_field_settings_form($field, $instance, $has_data) {

  // dpm($field);

  $form[$field['field_name']] = array(
    '#title' => t('Default map image'),
    '#description' => t('Add the default image/map for this field instance'),
    '#type' => 'managed_file',
    '#default_value' => ($field['settings']) ? $field['settings'][$field['field_name']] :  '' ,
    '#upload_location' => 'public://multipurpose_field_images/',
  );

  return $form;
}


/**
 * Implements hook_field_widget_form().
 */
function leaflet_map_field_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $element) {
  
  $image_path = '';
  $fid = $field['settings'][$field['field_name']] ;

  if($fid):
    $image = file_load($fid);
     if($image):
        $image_path = file_create_url($image->uri);
     endif;
  endif;

  $value = isset($items[$delta]['leaflet_coordinates']) ? $items[$delta]['leaflet_coordinates'] : '';
  $widget = $element;
  $widget['#delta'] = $delta;
  switch ($instance['widget']['type']) {  
    case 'field_coordinates_values':  

      $newvalue = new stdClass;
      $newvalue->image_path = $image_path;

      drupal_add_js( array('leaflet_map_field' => $newvalue), 'setting');

      $widget += array(
          '#type' => 'hidden',  
          '#title' => $element['#title'],  
          '#description' => $element['#description'], 
          '#default_value' => $value,  
          '#required' => $element['#required'],  
          '#weight' => isset($element['#weight']) ? $element['#weight'] : 0, 
          '#delta' => $delta,
          '#suffix' => '<div id="field_map" style="width: 700px; height: 500px; position: relative;"></div>',
          '#attributes' => array('class' => array('leaf-map-field')),
          '#attached' => array(
            'js' => array(drupal_get_path('module', 'leaflet_map_field') . '/js/leaflet_map_field.drupal.js')
        ),  
      );
    break; 
  }
  $element['leaflet_coordinates'] = $widget;

  return $element;
}

/**
 * Implements hook_field_widget_error().
 *
 */
function leaflet_map_field_field_widget_error($element, $error, $form, &$form_state) {
  switch ($error['error']) {
    case 'coordinates_data_invalid':
      form_error($element, $error['message']);
    break;
  }
}