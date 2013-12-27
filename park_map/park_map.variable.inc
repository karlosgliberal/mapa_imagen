<?php

include_once ('park_map.inc');

/**
 * @file
 *
 * This is the file with the variable options to fill as:
 */ 

/**
 * Implemente hook_variable_info()
 */

function park_map_variable_info($options) {


// options elements
  /* Select the node bundles */

  $list_bundles = _park_map_list_bundles();


  $variables['park_map_bundle'] = array(
    'type' => 'options',
    'title' => t('Select the node bundles to use', array(), $options),
    'options' => $list_bundles,
    // 'default' => '',
    'description' => t('Choose the node types to use in the interactive map.', array(), $options),
    'required' => FALSE,
    'group' => 'park_map_options',
    'localize' => TRUE,
  );


// Configurations elementes
  $variables['park_map_image'] = array(
    'type'              => 'upload_file',
    'title'             => t('My Upload File', array(), $options),
    'description'       => t('Upload my config file.', array(), $options),
    'upload_location'   => 'public://parkmap',
    'upload_validators' => array(
      'file_validate_extensions' => array('gif png jpg jpeg'),
    ),
    'group'             => 'park_map_configuration',
  );

  $variables['park_map_southwest_latlong'] = array(
    'type' => 'string',
    'title' => t('SouthWest Latitude', array(), $options),
   // 'default' => '',
    'description' => t('Write the SouthWest Latitude  Longitude for the park-map location fsafsafsaf.', array(), $options),
    'required' => TRUE,
    'group' => 'park_map_configuration',
  );

  $variables['park_map_northeast_latlong'] = array(
    'type' => 'string',
    'title' => t('NorthEast Latitude', array(), $options),
   // 'default' => '',
    'description' => t('Write the NorthEast Latitude Longitude for the park-map location.', array(), $options),
    'required' => TRUE,
    'group' => 'park_map_configuration',
  );


///////////////////////////////////////  Content type and Fields options configuration  /////////////////////////////////////////////////
// configurations of field by content type.
// I use the default variable_get because there is not way to use variable_get_value into  hook_variable_info()
$bundles =  variable_get('park_map_bundle', NULL);

// Here we have all the bundles-types that they are selected
$types = _park_map_bundles_selected($bundles);

// if there are bundles selected go on with the fields.
if($types) {

  foreach ($types as $type) {

    unset($fields_bundle);
    unset($field_list);

    $fields_bundle =  field_info_instances('node', $type);
    $field_list = array();

    foreach ($fields_bundle as $field) {
      $field_list[$field['field_name']] = $field['label'];
    }


    $variables['park_map_fields_' . $type ] = array(
      'type' => 'options',
      'title' => t('Select the fields of ' . $type, array(), $options),
      'options' => $field_list,
      // 'default' => '',
      'description' => t('Choose the node types to use in the interactive map.', array(), $options),
      'required' => FALSE,
      'group' => 'park_map_options',
      'localize' => TRUE,
    );  
  }
} 


//////////////////////////First Load /////////////////////////////////////
// Choose the content type and the the quantity of date to print in the first load.
  $variables['park_map_first_load_bundles'] = array(
    'type' => 'options',
    'title' => t('Select the node bundles to load', array(), $options),
    'options' => $list_bundles,
    // 'default' => '',
    'description' => t('Choose the node types to load in the first load of the map.', array(), $options),
    'required' => FALSE,
    'group' => 'park_map_first_load',
    'localize' => TRUE,
  );

  $variables['park_map_first_load_quantity'] = array(
    'type' => 'number',
    'title' => t('quantity.', array(), $options),
    //'options' => $list_bundles,
    'default' => 0,
    'description' => t('Select the quantity of content to load in the first load of the map.', array(), $options),
    'required' => FALSE,
    'group' => 'park_map_first_load',
    'localize' => TRUE,
  );

return $variables;
}



/**
 * Implements hook_variable_group_info()
 */
function park_map_varaible_group_info() {

  $groups['park_map_configuration'] = array (
    'title' => t('Park map configurations'),
    'descripcion' => t('Park map configuration options'),
    'access' => 'administer configuration',
    'path' => array('admin/config/park/park_configuration/configurations'),
  );

  $groups['park_map_options'] = array (
    'title' => t('Park map options'),
    'descripcion' => t('Park map configuration options'),
    'access' => 'administer configuration',
    'path' => array('admin/config/park/park_configuration/options'),
  );

  $groups['park_map_first_load'] = array (
    'title' => t('Park map first load'),
    'descripcion' => t('Park map configure first load'),
    'access' => 'administer configuration',
    'path' => array('admin/config/park/park_configuration/first_load'),
  );

  return $groups;
}