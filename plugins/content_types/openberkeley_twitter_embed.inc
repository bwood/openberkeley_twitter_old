<?php
$plugin = array(
  'title' => t('Twitter Embed Code'),
  'description' => t('Displays a Twitter embedded object.'),
  'single' => TRUE,
  'content_types' => array('openberkeley_twitter_embed'),
  'render callback' => 'openberkeley_twitter_openberkeley_twitter_embed_content_type_render',
  //'required context' => new ctools_context_required(t('Node'), 'node'),
  'edit form' => 'openberkeley_twitter_openberkeley_twitter_embed_content_type_edit_form',
  'category' => t('Embeddable content'),
  // custom elements
  'extract element callback' => 'openberkeley_twitter_widget_extract_element_callback',
  //'extract username callback' => 'openberkeley_twitter_widget_extract_username_callback',
  'extract username pattern' => '|href\s?=\s?"https://twitter.com/([^"]+)"|',
  //'extract data-widget-id callback' => 'openberkeley_twitter_widget_extract_data_widget_id_callback',
  'extract data-widget-id pattern' => '|data-widget-id\s?=\s?"([^"]+)"|',
  'embed template' => '<a class="twitter-timeline" href="https://twitter.com/[username]" data-widget-id="[data-widget-id]">Tweets by @[username]</a><script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?\'http\':\'https\';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+"://platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>',
);

/**
 * Ctools edit form.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function openberkeley_twitter_openberkeley_twitter_embed_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $form['openberkeley_twitter_embed_code'] = array(
    '#type' => 'textarea',
    '#title' => t('Twitter Embed Code'),
    '#description' => t('Paste your Twitter embed code here.'),
    '#default_value' => !empty($conf['openberkeley_twitter_embed_code']) ? $conf['openberkeley_twitter_embed_code'] : '',
  );
  return $form;
}

/**
 * Form validation callback for openberkeley_twitter_embed_content_type_edit_form().
 */
function openberkeley_twitter_openberkeley_twitter_embed_content_type_edit_form_validate($form, &$form_state) {
/*
  $subtype = $form_state['subtype_name'];
  $widgets = openberkeley_widgets_simple_embed_widgets();
  $widget = $widgets[$subtype];

  if (!call_user_func($widget['extract link callback'], $form_state['values']['link'], $widget)) {
    form_set_error('link', t('Invalid Google Forms link or embed code.'));
  }

  $height = $form_state['values']['height'];
  if (!is_numeric($height) || intval($height) != $height || intval($height) < 0) {
    form_set_error('height', t('Height must be a positive, whole number.'));
  }
*/
  return TRUE;
}
/**
 * Ctools edit form submit handler.
 *
 * @param $form
 * @param $form_state
 */
function openberkeley_twitter_openberkeley_twitter_embed_content_type_edit_form_submit($form, &$form_state) {
//  $subtype = $form_state['subtype_name'];
//  $widgets = openberkeley_widgets_simple_embed_widgets();
//  $widget = $widgets[$subtype];
//  global $plugin;


  // save the embed code for revalidation in the render stage
  $form_state['conf']['openberkeley_twitter_embed_code'] = $form_state['values']['openberkeley_twitter_embed_code'];
    //$form_state['conf']['username'] = call_user_func($plugin['extract element callback'], $form_state['values']['openberkeley_twitter_embed_code'], $plugin['extract username pattern']);
  $form_state['conf']['username'] = call_user_func('openberkeley_twitter_widget_extract_element_callback', $form_state['values']['openberkeley_twitter_embed_code'], '|href\s?=\s?"https://twitter.com/([^"]+)"|');
  //$form_state['conf']['data-widget-id'] = call_user_func($plugin['extract element callback'], $form_state['values']['openberkeley_twitter_embed_code'], $plugin['extract data-widget-id pattern']);
  $form_state['conf']['data-widget-id'] = call_user_func('openberkeley_twitter_widget_extract_element_callback', $form_state['values']['openberkeley_twitter_embed_code'], '|data-widget-id\s?=\s?"([^"]+)"|');
  //$form_state['conf']['height'] = $form_state['values']['height'];

}

/**
 * Render callback for the openberkeley_twitter_embed content type.
 */
function openberkeley_twitter_openberkeley_twitter_embed_content_type_render($subtype, $conf, $args, $context) {

  // For security purposes, we re-validate the data before rendering.
  //$conf['username'] = call_user_func($plugin['extract element callback'], $conf['openberkeley_twitter_embed_code'], $plugin['extract username pattern']);
  $conf['username'] = call_user_func('openberkeley_twitter_widget_extract_element_callback', $conf['openberkeley_twitter_embed_code'], '|href\s?=\s?"https://twitter.com/([^"]+)"|');
  //$conf['data-widget-id'] = call_user_func($plugin['extract element callback'], $conf['openberkeley_twitter_embed_code'], $plugin['extract data-widget-id pattern']);
  $conf['data-widget-id'] = call_user_func('openberkeley_twitter_widget_extract_element_callback', $conf['openberkeley_twitter_embed_code'], '|data-widget-id\s?=\s?"([^"]+)"|');

  if (empty($conf['username']) || empty($conf['data-widget-id'])) {
    return;
  }

  $template = '<a class="twitter-timeline" href="https://twitter.com/[username]" data-widget-id="[data-widget-id]">Tweets by @[username]</a><script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?\'http\':\'https\';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+"://platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>';
  // Generate the embed code.
  $embed = call_user_func('openberkeley_twitter_widget_embed_callback', $conf, $template);

  $pane = new stdClass();
  //$pane->content = '<a class="twitter-timeline" href="https://twitter.com/getbwood" data-widget-id="419169885516599296">Tweets by @getbwood</a><script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?\'http\':\'https\';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+"://platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>';
  $pane->content = $embed;
  return $pane;
}
