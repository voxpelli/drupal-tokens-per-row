<?php
// $Id$

/**
 * A handler to provide a field that is completely custom by the administrator
 * and evaluates tokens entered in a textarea on a per row basis. The first row
 * that contains data will be rendered and others ignored.
 * 
 * Based on views_handler_field_custom.inc.
 *
 * @ingroup views_field_handlers
 */
class tokens_per_row_handler_field_custom_tokens_per_row extends views_handler_field {
  function query() {
    // do nothing -- to override the parent query.
  }

  function option_definition() {
    $options = parent::option_definition();

    // Override the alter text option to always alter the text.
    $options['alter']['contains']['alter_text'] = array('default' => TRUE);
    return $options;
  }

  function options_form(&$form, &$form_state) {
    parent::options_form($form, $form_state);
    //The text to display for this field. You may include HTML. You may enter data from this view as per the "Replacement patterns" below.
    $form['alter']['text']['#title'] = t('Text (Replacement patterns)');
    $form['alter']['text']['#description'] = t('Enter data from "Replacement patterns" below. The patterns will be evaluated on a per row basis. The first row that contains data will be rendered and the other ones will be ignored.');
    // Remove the checkbox
    unset($form['alter']['alter_text']);
    unset($form['alter']['text']['#dependency']);
    unset($form['alter']['text']['#process']);
  }

  function render_altered($alter, $tokens) {
    $result = '';

    // Filter this right away as our substitutions are already sanitized.
    $text = filter_xss_admin(trim($alter['text']));

    // Split text by linebreak (\r, \r\n or \n).
    $lines = preg_split('/(\r\n?|\n)/', $text);
    foreach ($lines as $line) {
      $invert = FALSE;

      if (substr($line, 0, 1) == '!') {
        $line = substr($line, 1);
        if (substr($line, 0, 1) == '!') {
          $line = substr($line, 1);
        }
        else {
          $invert = TRUE;
        }
      }

      $value = trim(strtr($line, $tokens));

      // Return the first non-empty token replacement or empty inverted token replacement.
      if (empty($value)) {
        if ($invert) {
          $result = '';
          break;
        }
      }
      else if (!$invert) {
        $result = $value;
        break;
      }
    }

    // Default.
    return $result;
  }

  function render($values) {
    // Nothing to render.
    return '';
  }
}
