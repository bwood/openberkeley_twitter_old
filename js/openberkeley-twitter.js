(function ($) {
  // Hack to make the twttr.ready() function available before the script has
  // loaded. This was inspired from this snippet in the Twitter docs:
  //   https://dev.twitter.com/web/javascript/loading
  if (typeof twttr === 'undefined') {
    twttr = {
      _e: [],
      ready: function (f) {
        twttr._e.push(f);
      }
    };
  }

  // Bind to the Twitter widgets load event to add some custom CSS.
  twttr.ready(function () {
    twttr.events.bind('rendered', function (evt) {
      if (!evt.target.src) {
        $(evt.target).contents().find(".timeline").attr("style","max-width: 100% !important;");
        $(evt.target).attr("style","max-width: 100% !important; width: 100% !important;");
      }
    });
  });

  Drupal.behaviors.openberkeley_twitter = {
    attach: function (context, settings) {
      if ($(".twitter-timeline").length > 0) {
        if (typeof twttr.widgets !== 'undefined') {
          twttr.widgets.load();
        }
      }
    }
  };
})(jQuery);
