$(document).ready(function () {

  //___________________________________________________________________________
  // Local variables used to control re-rendering.

  // Get accessor for querying content after '?' in the URL.
  var urlParams   = new URLSearchParams(window.location.search);

  var timestamp   = new Date();     ///< Used by copyright.
  var debug  = (urlParams.get('debug') == 'true');
  var cover = {};                   ///< Used by waypoints to support coverage.
  var entity = {};                  ///< Storage for custom entities.

  //___________________________________________________________________________
  // search contains all search patterns used to re-render markDOWN as markUP.
  var search = {
    define  : {pattern: /&([A-Za-z][0-9A-Za-z]*)=([^;]+);/gim },
    verbatim: {pattern: /!V([^]+)V!/},
    entities: {pattern: /(&amp;([A-Za-z][0-9A-Za-z]*);)/gim},
    commentN: {pattern: /(!#[^]*#!)/gim,
               replace: ''},
    comment1: {pattern: /(!##[^]*$)/gim,
               replace: ''},
    indent  : {pattern: /^\s*\.\.\.(.*)$/gim,
               replace: '&nbsp;&nbsp;&nbsp;$1'},
    heads   : {
        index: [6, 5, 4, 3, 2, 1],  ///< inverse order of indices required.
        1: {pattern: /^\s*=\s*(.+)\s*=\s*$/gim,
            replace: 
                '<div align="center">' +
                '<big>'.repeat(6) + '$1' + '</big>'.repeat(6) +
                '</div>'
        },
        2: {pattern: /^\s*==\s*(.+)\s*==\s*$/gim,
            replace: 
                '<div align="center">' +
                '<big>'.repeat(5) + '$1' + '</big>'.repeat(5) +
                '</div>'
        },
        3: {pattern: /^\s*===\s*(.+)\s*===\s*$/gim,
            replace: 
                '<div align="center">' +
                '<big>'.repeat(4) + '$1' + '</big>'.repeat(4) +
                '</div>'
        },
        4: {pattern: /^\s*====\s*(.+)\s*====\s*$/gim,
            replace: 
                '<div align="center">' +
                '<big>'.repeat(3) + '$1' + '</big>'.repeat(3) +
                '</div>'
        },
        5: {pattern: /^\s*=====\s*(.+)\s*=====\s*$/gim,
            replace: 
                '<div align="center">' +
                '<big>'.repeat(2) + '$1' + '</big>'.repeat(2) +
                '</div>'
        },
        6: {pattern: /^\s*======\s*(.+)\s*======\s*$/gim,
            replace: 
                '<div align="center">' +
                '<big>'.repeat(1) + '$1' + '</big>'.repeat(1) +
                '</div>'
        }
    },
    enhance : {
        b: {pattern: /!=(.*)=!/g,
            replace: '<b>$1</b>'   },
        i: {pattern: /!\/(.*)\/!/g,
            replace: '<i>$1</i>'   },
        s: {pattern: /!-(.*)-!/g,
            replace: '<s>$1</s>'   },
        u: {pattern: /!_(.*)_!/g,
            replace: '<u>$1</u>'   }
        },
    space   : {
      index : "hr2,hr1,p,br".split(','),
      hr2   : {pattern: /_{4,}/g,
               replace: '<hr /><hr />'},
      hr1   : {pattern: /_{3,}/g,
               replace: '<hr />'      },
      p     : {pattern: /\^\^\^\^/g,
               replace: '<p />'       },
      br    : {pattern: /\^\^\^/g,
               replace: '<br />'      }
    },
    align   : {
      center: {pattern: /!C([^]+?)C!/gim,
               replace: '<div align="center">$1</div>'},
      left  : {pattern: /!L([^]+?)L!/gim,
               replace: '<div align="left">$1</div>'},
      right : {pattern: /!R([^]+?)R!/gim,
               replace: '<div align="right">$1</div>'},
    },
    bullets: {
        index: [6, 5, 4, 3, 2, 1],  ///< inverse order of indices required.
        1: {pattern: /^\s*\*/gim,
            replace: '<br />' + '&nbsp;&nbsp;'.repeat(1) + '&#8226; ' },
        2: {pattern: /^\s*\*\*/gim,
            replace: '<br />' + '&nbsp;&nbsp;'.repeat(2) + '&spades; '},
        3: {pattern: /^\s*\*\*\*/gim,
            replace: '<br />' + '&nbsp;&nbsp;'.repeat(3) + '&hearts; '},
        4: {pattern: /^\s*\*\*\*\*/gim,
            replace: '<br />' + '&nbsp;&nbsp;'.repeat(4) + '&diams; ' },
        5: {pattern: /^\s*\*\*\*\*\*/gim,
            replace: '<br />' + '&nbsp;&nbsp;'.repeat(5) + '&clubs; ' },
        6: {pattern: /^\s*\*\*\*\*\*\*/gim,
            replace: '<br />' + '&nbsp;&nbsp;'.repeat(6) + '&dagger; '}
      },
    // TODO numberlist
    check  : {
      pattern: /^\s*!@(.)(.)\s*([^]*?)@!\s*$/gim,
      box  : {
        ' ': '<b>&#x2610;</b>',
        'x': '<b style="color:red">&#x2612;</b>',
        'v': '<b style="color:green">&#x2611;</b>'
      },
      num  : {
        ' ': '&nbsp;',
        '0': '&#9450;',
        '1': '&#9312;',
        '2': '&#9313;',
        '3': '&#9314;',
        '4': '&#9315;',
        '5': '&#9316;',
        '6': '&#9317;',
        '7': '&#9318;',
        '8': '&#9319;',
        '9': '&#9320;'
      }
    }
  };

  //___________________________________________________________________________
  var waypoint = function(key, sub) {
    if (!key) {
      console.log(cover);
      Array.prototype.forEach.call(cover, function(key) {
        Array.prototype.forEach.call(cover[key], function(sub) {
          var count = cover[key][sub];
          if (count == 0) console.log("no coverage[" + key + "][" + sub + "]");
        });
      });
    } else {
      if (!Object.hasOwnProperty.call(cover, key)) cover[key] = {};
      if (!Object.hasOwnProperty.call(cover[key], sub)) cover[key][sub] = 0;
      cover[key][sub]++;
    }
  };

  //___________________________________________________________________________
  // Simple handling of waypoint data over all aspects of rendering.
  var follow = function(key, fun) {
    waypoint('cover', key);
    fun(key);
  }

  //___________________________________________________________________________
  // Fetch execution time to update the copyright above the nav.
  if (!Date.now) {
    Date.now = function now() {
      return timestamp.getTime();
    };
  }

  //___________________________________________________________________________
  // Unused but extremely powerful mechanism for implementing multiline text.
  function HEREDOC(f) {
    return f.toString().split('\n').slice(1,-1).join('\n').normalize('NFC');
  }

  //___________________________________________________________________________
  // Find and ingest "&{name}={replacement text};" definitions from CDATA.
  follow("define", function(name) {
    var my = search[name];
    var element = document.getElementById(name);
    // No replacement happens.  The replace function pattern is useful.
    element.innerHTML.replace(
      my.pattern,
      function(match, p1, p2, off, str) {
        waypoint(name, name);
        entity[p1] = p2;
        return '';
      });
  });

  //___________________________________________________________________________
  // markup is the central function for re-rendering markdown as markup.
  //___________________________________________________________________________
  var markup = function(section) {

    var source = section.innerHTML;  ///< Original markDOWN from a section.
    var target = source;             ///< Intended markUP to fill a section.

    // Potential logging and waypoint too.
    var verbose = function(msg) {
      console.log(msg);
    };

    //_________________________________________________________________________
    // Find and eliminate multi-line comment data
    follow("commentN", function(name) {
      var my = search[name];
      target = target.replace(my.pattern, my.replace);
    });

    //_________________________________________________________________________
    // Find and eliminate single-line comment data
    follow("comment1", function(name) {
      var my = search[name];
      target = target.replace(my.pattern, my.replace);
    });

    //_________________________________________________________________________
    // Verbatim guaranteed by conversion to explicit numeric codepoints.
    follow("verbatim", function(name) {
      var my = search[name];
      target = target.replace(my.pattern,
        function(match, p1, off, str) {
          waypoint(name, name);
          var result = "";
          var N = p1.length;
          for (i=0; i<N; ++i) {
            var chr = p1[i];
            var ord = p1.charCodeAt(i);
            if (ord == 0x0a) result += chr;
            else result += "&#" + ord.toString() + ";";
          }
          return '<pre>' + result + '</pre>';
        });
    });

    //_________________________________________________________________________
    // Replace custome entities produced by the "defined" stage.
    follow("entities", function(name) {
      var my = search[name];
      target = target.replace(my.pattern,
        function(match, p1, p2, off, str) {
          waypoint(name,p2);
          if (Object.hasOwnProperty.call(entity, p2)) return entity[p2];
          else return p1;
        });
    });

    //_________________________________________________________________________
    // Indent paragraphs
    follow("indent", function(name) {
      var the = search[name];
      target = target.replace(the.pattern, the.replace);
    });

    //_________________________________________________________________________
    // H1, H2, H3, H4, H5, H6
    follow("heads", function(name) {
      var my = search[name];
      var head = function(the) {
        target = target.replace(the.pattern, the.replace);
      };
      Array.prototype.forEach.call(my.index, function(i) { head(my[i]); });
    });

    //_________________________________________________________________________
    // Bold, Italic, Underscore, Strike
    follow("enhance", function(name) {
      var my = search[name];
      var enhance = function(the) {
        target = target.replace(the.pattern, the.replace);
      };
      for (var index in my) enhance(my[index]);
    });

    //_________________________________________________________________________
    // Horizontal rule, line, and paragraph breaks
    // TODO insert waypoint.
    // TODO use function rather than .replace
    follow("space", function(name) {
      var my = search[name];
      for (var index in my.index) {
        var the = my[my.index[index]];
        target = target.replace(the.pattern, the.replace);
      }
    });

    //_________________________________________________________________________
    // Right/Center/Left alignment
    follow("align", function(name) {
      var my = search[name];
      var alignK = function(key) {
        var the = my[key];
        target = target.replace(the.pattern, the.replace);
      };
      for (align in my) alignK(align);
    });

    //_________________________________________________________________________
    // Bullet lists
    follow("bullets", function(name) {
      var my = search[name];
      var bulletL = function(the) {
        target = target.replace(the.pattern, the.replace);
      };
      for (var index in my.index) bulletL(my[my.index[index]]);
    });

    //_________________________________________________________________________
    // Number lists (TODO)
    follow("number", function(name) {
      var numbering = [0, 0, 0, 0, 0];
      var numberL = function(level) {
        lines = target.split('\n');
        for (line in lines) {
        }
      };
    });

    //_________________________________________________________________________
    // Punch lists
    follow("check", function(name) {
      var my = search[name];
      target = target.replace(my.pattern,
        function(match, p1, p2, p3, off, str) {
          var chr1 = my.box[p1] || p1;
          var chr2 = my.num[p2] || p2;
          return '' +
            '<div class="check">' +
              '<div class="check-box">'      + chr1 + '</div>' +
              '<div class="check-priority">' + chr2 + '</div>' +
              '<div class="check-text">'     + p3   + '</div>' +
            '</div>';
        });
    });

    //_________________________________________________________________________
    // If debugging, show the markdown after the markup
    follow("debug", function(name) {
      var text = '';
      if (debug) {
        waypoint(name, name);
        text =
          '<hr />' +
          '<div align="left" style="color:white; background-color:black;">' +
          '<pre>\n' +
          source +
          '</pre>' +
          '</div>';
      }
      target += text;
    });

    section.innerHTML = target;
  };
  //___________________________________________________________________________
  // markup function ends here.
  //___________________________________________________________________________

  //___________________________________________________________________________
  // HTML5 Header
  follow("HTML5:header", function(name) {
    markup(document.getElementsByTagName("header")[0]);
  });

  //___________________________________________________________________________
  // HTML5 Nav
  follow("HTML5:section", function(name) {
    var menu        = document.getElementsByTagName("nav")[0];
    var sections    = document.getElementsByTagName("section");
    var page        = document.getElementById(urlParams.get('page'));
    page            = page ? page : sections[0];
    var pageid      = page.getAttribute("id");
    var count       = 0;
    var perline     = 5;

    // Generate the list of sections to populate the Nav bar.
    var html =
      '<small><small><small><small>' +
      'Copyright&copy; 2016-' +
      timestamp.getFullYear() +
      ' Jonathan D. Lettvin, All Rights Reserved' +
      '</small></small></small></small>';
    html +=
      '<table class="nav" align="center"><tr>';
    Array.prototype.forEach.call(sections, function(section) {
      var style = [
        {'td': 'background-color:white;', 'a': 'color:black;'},
        {'td': 'background-color:black;', 'a': 'color:white;'}];
      var id = section.getAttribute("id");
      var chosen = + (pageid == id);
      var tdstyle = style[chosen]['td'];
      var astyle = style[chosen]['a'];
      waypoint(name, id);
      if ((count % perline) == 0) html += '</tr><tr>';
      count++;
      html +=
        '<td class="nav" align="center" style="' + tdstyle + '">' +
        '<a href="?page=' + id + '" style="' + astyle + '">' +
        '<big>' + id + '</big>' +
        '</a>' +
        '</td>';
      section.style.display = "none";
    });
    html += "</tr></table>";

    // If not debugging, give opportunity to debug
    if (!debug) {
        html += 
        '<a href="?page=' + pageid + '&debug=true">' +
        '<small>debug</small>' +
        '</a>';
    }

    // Show the menu.
    if (menu) menu.innerHTML = html;

    // Only show the chosen page.
    if (page) {
      page.style.display = "inline";
      markup(page);
    }
  });

  //___________________________________________________________________________
  // HTML5 Footer
  follow("HTML5:footer", function(name) {
    var elements    = document.getElementsByTagName("footer");
  });

  waypoint(null);
});
