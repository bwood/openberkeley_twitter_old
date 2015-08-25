(function ($) {
  Drupal.behaviors.openberkeley_twitter = {
    attach: function (context, settings) {
      if (typeof twttr !== 'undefined' && typeof twttr.widgets !== 'undefined') {
        twttr.widgets.load();
      }
    }
  };
})(jQuery);
