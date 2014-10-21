// Make 'window' the global scope
self = window = this;
window.top = window.parent = window;

var berry = window.__BERRY__ = {};

(function(window) {

    var animFrameCallbacks = [], nextAnimFrameCallbackID = 0;
    berry.tickAnimFrame = function() {
        for (var i = 0, len = animFrameCallbacks.length; i < len; i++) {
            animFrameCallbacks[i].callback();
        }
    };

    window.requestAnimationFrame = function(cb) {
        animFrameCallbacks.push({
            id: nextAnimFrameCallbackID++,
            callback: cb
        });
        return nextAnimFrameCallbackID;
    };
    window.cancelAnimationFrame =function(id) {
        for (var i = 0, len = animFrameCallbacks.length; i < len; i++) {
            if (animFrameCallbacks[i].id === id) {
                animFrameCallbacks.splice(i, 1);
            }
        }
    };

})(this);
