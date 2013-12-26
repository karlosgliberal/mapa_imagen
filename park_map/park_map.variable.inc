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

// configurations of field by content type.
// I use the default variable_get because there is not way to use variable_get_value into  hook_variable_info()
$types =  variable_get('park_map_bundle', NULL);

// validate if there are elementes selecteds
$there_is_bundles = _park_map_bundles_selected($types);

// if there are bundles selected go on with the fields.
if($there_is_bundles) {

  foreach ($types as $type) {

    if($type || $type !=NULL){

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
} 


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


  return $groups;
}