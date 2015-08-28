(function ($) {
  var twitterBackup = {};

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

  // Get the iframe document in a cross-browser way.
  function getIframeDocument(iframe) {
    return iframe.document || iframe.contentDocument || iframe.contentWindow.document;
  }

  // Get the contents of an iframe in a cross-browser way.
  function getIframeContent(iframe) {
    return getIframeDocument(iframe).documentElement.innerHTML;
  }

  // Copies the contents of one iframe to another.
  function replaceIframeContent(dst, content) {
    var frameDocument = getIframeDocument(dst);

    frameDocument.open();
    frameDocument.writeln('<html>');
    frameDocument.writeln(content);
    frameDocument.writeln('</html>');
    frameDocument.close();
  }

  Drupal.behaviors.openberkeley_twitter = {
    attach: function (context, settings) {
      // Load any new Twitter widgets that were added to the page.
      if (typeof twttr.widgets !== 'undefined') {
        twttr.widgets.load();
      }

      // Bind to the 'Customize this page' button if present, so we can fix
      // the Twitter widegts stored in the IPE backup.
      $('#panels-ipe-customize-page', context).once('openberkeley-twitter').click(function () {
        var container = $(this).closest('.panels-ipe-control').get(0),
            editorId = container.id.substr(19),
            editor = Drupal.PanelsIPE.editors[editorId],
            cancelIPE = editor.cancelIPE;

        // First, fill in the backup so that it contains the full contents
        // of the original iframe.
        $('.twitter-timeline', editor.backup).each(function () {
          twitterBackup[this.id] = getIframeContent(this);
        });

        // Then we have to monkey patch the cancel function so we can make
        // sure the copy also has the full contents.
        if (!editor.cancelIPE.monkeyPatched) {
          editor.cancelIPE = function () {
            // Call the parent function.
            cancelIPE();

            // Now, fill in the iframe on the page with the full contents of
            // the backup.
            setTimeout(function () {
              $('.twitter-timeline').each(function () {
                if (twitterBackup[this.id]) {
                  replaceIframeContent(this, twitterBackup[this.id]);
                }
              });
            }, 1000);

          };
          editor.cancelIPE.monkeyPatched = true;
        }
      });
    }
  };
})(jQuery);
