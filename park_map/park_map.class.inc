<?php

/**
 * @File
 *
 * This file store the class that will help us to build the dinamic querys
 *
 * It's a layer over db_select
 *
 */
/*
class ParkMapQuery {

   
  function __construct() {

    $query = db_select('node', 'n');
    $query->condition('status', 1, '=');
    $query->fields('n', array('title','body'));

    return $query;

  }

  function SetQueryObject() {

    return $this->query;

  }

  function QueryBuilderLang($lang, $query) {
    dpm($query);
    try{
      if(empty($lang)){

        throw new Exception('Lang variable is empty', 1);
        

      } else {

        return $query->condition('n._language', $lang, '=');
      }
    } catch(Exception $e){
      drupal_set_message(
        t('La variable de idioma en el método QueryBuilderLang esta vacia'), 
        'error');
    }

  }


  function QueryBuilderOr($values,$query) {

    
    try {
      
      if(!is_array($values)){
        throw new Exception('La variable $value del método QueryBuilderOr no es un array');
      } else {

        $db_or = db_or(); 
        foreach ($values as  $value) {
          $db_or->condition('n.type', $value, '=');
        }
        return $query->condition($db_or);
      }
  
    }catch (Exception $e){
      
      drupal_set_message(
        t('ParkMap query builder OR = %message', array('%message' => $e->getMessage())),   'error');
    }
  }


  function QueryBuilderExecute($query){

    return $query->execute();

  }
  function QueryBuilderDebug($query) {

    return $query->__toString();

  }

}



$types = array(
'article',
'banner-big',
'event'
);

$query = db_select('node','n');

$query->condition( 'n.status' , 1 );

if($types && is_array($types)){
$db_or = db_or();
foreach($types as $type)
{
$db_or->condition('n.type',  $type , '=' );
}
$query->condition($db_or);
}

$query->leftJoin('field_data_body','fb', 'n.nid = fb.entity_id');

$query->fields('n',array('title'));

$query->fields('fb', array('body_value'));

$query->range(0,10);

$valores = $query->execute();

dpm($query->__toString());

foreach($valores as $valor){

dpm($valor);
}
}

*/

