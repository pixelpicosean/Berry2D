print('Current time is: ' + Date.now());

print('Math.random() = ' + Math.random());
print('Math.sin(Math.PI) = ' + Math.sin(Math.PI));

print('window.innerWidth = ' + window.innerWidth);
print('window.innerHeight = ' + window.innerHeight);

var count = 0;
window.requestAnimationFrame(function() {
    count++;
    if (count > 60) {
        print('tick 60 times');
        count = 0;
    }
});
