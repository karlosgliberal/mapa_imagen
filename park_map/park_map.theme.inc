<?php

/**
 * @file
 *
 * All the theming options implements in this module
 */

/**
 * Implement hook_theme()
 */
function park_map_theme() {

 return array (
    'render_park_map' => array (
      // 'template' => 'page--park-map',
      'template' => 'render-park-map',
      'variables' => array ('values' => NULL),
    ),
  );
}

/**
 * Implemente templete preprocess for the tpl.
 */

function template_preprocess_render_park_map(&$variables) {


/*
  $variables['park_map_path'] = $variables[0]->image_absolute_path;

  // Generamos variables por latitudes/longitudes
  foreach ( $variables[0]->geolocation as $key => $value) {
    $variables[$key] = $value;  
  }
*/
}