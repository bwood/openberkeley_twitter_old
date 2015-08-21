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
);

/**
 * Ctools edit form.
 *
 * @param $form
 * @param $form_state
 * @return mixed
 */
function openberkeley_twitter_openberkeley_twitter_embed_content_type_edit_form($form, &$form_state) {

  $instructions =<<<EOT
 <div>
   <ol>
    <li>Login to Twitter using the account for which you'd like to embed data.</li>
    <li>Visit https://twitter.com/settings/widgets and choose "Create New."</li>
    <li>Recommended: Uncheck auto-expand photos.</li>
    <li>Recommended: Do not change height, theme or link color.</li>
    <li>Click the "Save Changes" button.</li>
    <li>Select all of the code in the box appearing above "Copy and paste the code into the HTML of your site."</li>
    <li>Paste the code into the "Twitter embed code" box here.</li>
    <li>Click Save.</li>
  </ol>
 </div>
EOT;

  $conf = $form_state['conf'];
  $form['openberkeley_twitter_container'] = array(
    '#type' => 'fieldset',
    '#tree' => FALSE,
    '#title' => t('Twitter Settings'),
    '#description' => t(''),
    '#collapsible' => FALSE,
    '#collapsed' => FALSE,
    '#weight' => -100
  );
  $form['openberkeley_twitter_container']['openberkeley_twitter_instructions_text'] = array(
    '#markup' => $instructions,

  );
  $form['openberkeley_twitter_container']['openberkeley_twitter_number_tweets'] = array(
    '#type' => 'select',
    '#required' => TRUE,
    '#title' => t('Number of tweets to display'),
    '#description' => t(''),
    '#default_value' => !empty($conf['openberkeley_twitter_number_tweets']) ? !empty($conf['openberkeley_twitter_number_tweets']) : 3,
    '#empty_option' => t('- Select -'),
    '#options' => array(1=>1,2=>2,3=>3,4=>4,5=>5,6=>6,7=>7,8=>8,9=>9,10=>10,11=>11,12=>12,13=>13,14=>14,15=>15,16=>16,17=>17,18=>18,19=>19,20=>20),
  );
  $form['openberkeley_twitter_container']['openberkeley_twitter_embed_code'] = array(
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
  $widget = openberkeley_twitter_widget();

  // Don't expect the embed code/patterns to change so hardcoding...
  $patterns = array(
    'username',
    'data-widget-id',
    'title'
  );
  foreach ($patterns as $type) {
    if (!call_user_func($widget['extract element callback'], $form_state['values']['openberkeley_twitter_embed_code'], $widget["extract $type pattern"])) {
      form_set_error('link', t('Invalid Twitter embed code.'));
      return TRUE;
    }
  }

  /*
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
  $widget = openberkeley_twitter_widget();

  // save the embed code for revalidation in the render stage
  $form_state['conf']['openberkeley_twitter_embed_code'] = $form_state['values']['openberkeley_twitter_embed_code'];
  $form_state['conf']['username'] = call_user_func($widget['extract element callback'], $form_state['values']['openberkeley_twitter_embed_code'], $widget['extract username pattern']);
  $form_state['conf']['data-widget-id'] = call_user_func($widget['extract element callback'], $form_state['values']['openberkeley_twitter_embed_code'], $widget['extract data-widget-id pattern']);
  $form_state['conf']['openberkeley_twitter_number_tweets'] = $form_state['values']['openberkeley_twitter_number_tweets'];

}

/**
 * Render callback for the openberkeley_twitter_embed content type.
 */
function openberkeley_twitter_openberkeley_twitter_embed_content_type_render($subtype, $conf, $args, $context) {
  $widget = openberkeley_twitter_widget();

  // For security purposes, we re-validate the data before rendering.
  $conf['username'] = call_user_func($widget['extract element callback'], $conf['openberkeley_twitter_embed_code'], $widget['extract username pattern']);
  $conf['data-widget-id'] = call_user_func($widget['extract element callback'], $conf['openberkeley_twitter_embed_code'], $widget['extract data-widget-id pattern']);
  $conf['title'] = call_user_func($widget['extract element callback'], $conf['openberkeley_twitter_embed_code'], $widget['extract title pattern']);


  if (empty($conf['username']) || empty($conf['data-widget-id']) || empty($conf['title'])) {
    return;
  }

  // Generate the embed code.
  $embed = call_user_func($widget['embed callback'], $conf, $widget['embed template']);

  $pane = new stdClass();
  //$pane->content = '<a class="twitter-timeline" href="https://twitter.com/getbwood" data-widget-id="419169885516599296">Tweets by @getbwood</a><script>!function(d,s,id){var js,fjs=d.getElementsByTagName(s)[0],p=/^http:/.test(d.location)?\'http\':\'https\';if(!d.getElementById(id)){js=d.createElement(s);js.id=id;js.src=p+"://platform.twitter.com/widgets.js";fjs.parentNode.insertBefore(js,fjs);}}(document,"script","twitter-wjs");</script>';
  $pane->content = $embed;
  return $pane;
}
