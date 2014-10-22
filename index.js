print('Current time is: ' + Date.now());

print('Math.random() = ' + Math.random());
print('Math.sin(Math.PI) = ' + Math.sin(Math.PI));

print('window.innerWidth = ' + window.innerWidth);
print('window.innerHeight = ' + window.innerHeight);

print('window.navigator.language = ' + window.navigator.language);
print('window.navigator.userAgent = ' + window.navigator.userAgent);
print('window.navigator.appVersion = ' + window.navigator.appVersion);
print('window.navigator.platform = ' + window.navigator.platform);

var count = 0;
window.requestAnimationFrame(function() {
    count++;
    if (count > 60) {
        print('tick 60 times');
        count = 0;
    }
});
