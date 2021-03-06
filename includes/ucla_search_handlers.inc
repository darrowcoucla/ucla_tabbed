<?php
/**
 * @file
 * Defines routines for searching various custom targets.
 */

/**
 * Route searches to appropriate target handler.
 */
function ucla_search_route_search($search_info) {
  // Docs say drupal_goto is for "local" URLs but works fine with absolute off-site URLs too
  // ... is there a better choice? Maybe drupal_http_request for GET/POST consistency?

  switch ($search_info['search_target']) {
    case "catalog":
      ucla_search_catalog($search_info);
      break;
    case "reserves":
      ucla_search_reserves($search_info);
      break;
    case "articles":
      serial_solutions_360($search_info);
      break;
    case "database":
      ucla_search_database($search_info);
      break;
    case "guides":
      ucla_search_guides($search_info);
      break;
    case "melvyl":
      ucla_search_melvyl($search_info);
      break;
    case "oac":
      ucla_search_oac($search_info);
      break;
    case "sfx":
      ucla_search_sfx($search_info);
      break;
    case "site":
      ucla_search_site($search_info);
      break;
    default:
      // TODO: change this to write error to log
      drupal_set_message("WHAT???");
  }
}

function ucla_search_catalog($search_info) {
  $target_url = 'http://catalog.library.ucla.edu/cgi-bin/Pwebrecon.cgi?DB=local&Search_Arg=';
  $extra_params = '&Search_Code=';
  if ($search_info['search_code']) {
    $extra_params .= $search_info['search_code'];
  } else {
    $extra_params .= 'GKEY^';
  }
  $extra_params .= '&SL=None&CNT=50&HIST=1';
  // Replace space with plus, but do NOT url encode the terms
  $target_url .= str_replace(' ', '+', $search_info['search_terms']);
  // Extra params must be after the search terms in the URL
  $target_url .= $extra_params;
  drupal_goto($path = $target_url);
}

/*
<OPTION VALUE="A&O SCI:  Atmospheric and Oceanic Sciences (21)">A&O SCI:Atmospheric and Oceanic Sciences</OPTION>
A%26O+SCI%3A++Atmospheric+and+Oceanic+Sciences+(21)
A%26O+SCI%3A++Atmospheric+and+Oceanic+Sciences+%2821%29

http://catalog.library.ucla.edu/cgi-bin/Pwebrecon.cgi?INST=
Any
&DEPT=A%26O+SCI%3A++Atmospheric+and+Oceanic+Sciences+%2821%29
&CRSE=Any
&PID=fR6jFI80JtmI5CE6XoKNs1GJlefRYOL
&SEQ=20131127230710
&CNT=50&HIST=1
*/

function ucla_search_reserves($search_info) {
  $search_inst = urlencode( $search_info['search_inst'] );
  $search_dept = urlencode( $search_info['search_dept'] );
  $search_crse = urlencode( $search_info['search_crse'] );
  $base_url = 'http://catalog.library.ucla.edu/cgi-bin/Pwebrecon.cgi';
  $target_url = 'http://catalog.library.ucla.edu/cgi-bin/Pwebrecon.cgi?INST=';
  $target_url .= $search_inst;
  $target_url .= '&DEPT=';
  $target_url .= $search_dept;
  $target_url .= '&CRSE=';
  $target_url .= $search_crse;
  $target_url .= '&PID=';
  $target_url .= $search_info['search_pid'];
  $target_url .= '&SEQ=';
  $target_url .= $search_info['search_seq'];
  $target_url .= '&CNT=50&HIST=1';
  drupal_goto($path = $target_url);

//print_r($target_url);
//break;


/*
drupal_goto($base_url, array(
    'query' => array(
        'INST' => str_replace(' ', '+', $search_info['search_inst']),
        'DEPT' => str_replace(' ', '+', $search_info['search_dept']),
        'CRSE' => str_replace(' ', '+', $search_info['search_crse']),
        'PID' => $search_info['search_pid'],
        'SEQ' => $search_info['search_seq'],
        'CNT' => '50',
        'HIST' => '1',
    ),
));
*/


}

function serial_solutions_360($search_info) {
  $target_url = 'http://mb5hp5vz2c.cs.serialssolutions.com/results';
  $extra_params = '?SS_LibHash=MB5HP5VZ2C&locale=en-us&action=start&dbIDList=&term=';
  $extra_params .= str_replace(' ', '+', $search_info['search_terms']);
  $extra_params .= '&field0=';
  $extra_params .= $search_info['search_code'];
  $extra_params .= '&boolop1=and&term1=&field1=keyword&boolop2=and&term2=&field2=keyword&boolop4=And&term4=&field4=';
  $extra_params .= $search_info['search_year'];
  $extra_params .= str_replace(' ', '&catID=', $search_info['search_categories']);
  $target_url .= $extra_params;
  drupal_goto($path = $target_url);
}

function ucla_search_database($search_info) {
/*****
  // 2013-08-30 akohler: having trouble getting POST to work, but
  // keeping this code as a sample in case other forms require POST.
  $target_url = 'http://unitdev.library.ucla.edu/search/databases-title-result-AK.cfm';
  // This currently requires an HTTP POST, so can't use drupal_goto which is GET only
  $headers = array('Content-Type' => 'application/x-www-form-urlencoded'); // required for POST
  $data = array(
    'FORM.keyword' => $search_info['search_terms'],
    'FORM.method' => 'contains', // contains or starts // TEMP TEST
  );
  watchdog('SEARCH_POST', http_build_query($data));
  $response = drupal_http_request($target_url, $headers, 'POST', http_build_query($data));
  watchdog('SEARCH_RESPONSE', $response->request . ' *** ' . $response->redirect_code);
*****/

  $target_url = 'http://unitproj.library.ucla.edu/search/databases-title-result.cfm';
  // Replace space with plus, but do NOT url encode the terms
  $target_url .= '?keyword=' . str_replace(' ', '+', $search_info['search_terms']);
  if ($search_info['search_type']) {
    $target_url .= '&method=' . map_database_search_type($search_info['search_type']);
  }
  drupal_goto($path = $target_url);
}

function ucla_search_guides($search_info) {
  $target_url = 'http://guides.library.ucla.edu/search.php?iid=261&c=0&search=';
  // Replace space with plus, but do NOT url encode the terms
  $target_url .= str_replace(' ', '+', $search_info['search_terms']);
  drupal_goto($path = $target_url);
}

function ucla_search_melvyl($search_info) {
  $target_url = 'http://ucla.worldcat.org/search?qt=wc_org_ucla&q=';
  // Don't replace space with plus - URL-encode as-is
  $target_url .= drupal_encode_path($search_info['search_terms']);
  drupal_goto($path = $target_url);
}

function ucla_search_oac($search_info) {
  // Online Archive of California
  $target_url = 'http://www.oac.cdlib.org/search?query=';
  // Replace space with plus, but do NOT url encode the terms
  $target_url .= str_replace(' ', '+', $search_info['search_terms']);
  drupal_goto($path = $target_url);
}

function ucla_search_sfx($search_info) {
  $target_url = 'http://ucelinks.cdlib.org:8888/sfx_ucla/az/default';
  $extra_params = '?param_perform_value=searchTitle&param_jumpToPage_value=&param_type_value=textSearch&param_chinese_checkbox_active=1&param_chinese_checkbox_value=0';
  $target_url .= $extra_params;
  // Replace space with plus, but do NOT url encode the terms
  $target_url .= '&param_pattern_value=' . str_replace(' ', '+', $search_info['search_terms']);
  if ($search_info['search_type']) {
    $target_url .= '&param_textSearchType_value=' . map_sfx_search_type($search_info['search_type']);
  }
  drupal_goto($path = $target_url);
}

function ucla_search_site($search_info) {
  $target_url = 'search/node/' . trim($search_info['search_terms']);
  drupal_goto($path = $target_url);
}

// Utility functions

function map_database_search_type($ucla_value) {
  // ucla form value => databases required value
  $mappings = array (
    'within' => 'contains',
    'starts' => 'starts',
  );
  return $mappings[$ucla_value];
}

function map_sfx_search_type($ucla_value) {
  // ucla form value => sfx required value
  $mappings = array (
    'within' => 'contains',
    'exact' => 'exactMatch',
    'starts' => 'startsWith',
  );
  return $mappings[$ucla_value];
}