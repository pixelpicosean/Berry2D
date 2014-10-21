print('Current time is: ' + Date.now());

var count = 0;
window.requestAnimationFrame(function() {
    count++;
    if (count > 60) {
        print('tick 60 times');
        count = 0;
    }
});
