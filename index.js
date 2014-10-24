print('Current time is: ' + Date.now());

print('Math.random() = ' + Math.random());
print('Math.sin(Math.PI) = ' + Math.sin(Math.PI));

print('window.innerWidth = ' + window.innerWidth);
print('window.innerHeight = ' + window.innerHeight);

print('window.navigator.language = ' + window.navigator.language);
print('window.navigator.userAgent = ' + window.navigator.userAgent);
print('window.navigator.appVersion = ' + window.navigator.appVersion);
print('window.navigator.platform = ' + window.navigator.platform);

console.debug('debug info');
console.info('info info');
console.warn('warn info');
console.error('error info');
console.log('log info');

var count = 0;
window.requestAnimationFrame(function() {
    count++;
    if (count > 60) {
        print('tick 60 times');
        count = 0;
    }
});

window.setTimeout(function() {
    print('setTimeout (1000ms)');
}, 1000);

var intervalCount = 0;
var loopID = window.setInterval(function() {
    print('setInterval (1600ms), called ' + (intervalCount + 1) + ' times');
    intervalCount++;
    if (intervalCount > 2) {
        print("clearInterval [" + loopID + '] after ' + intervalCount + ' times');
        window.clearInterval(loopID);
    }
}, 1600);

document.addEventListener('DOMContentLoaded', function() {
    print("event: DOMContentLoaded");
    document.dispatchEvent(document.createEvent('custom'));
});
document.addEventListener('custom', function() {
    print("event: custom");
});


localStorage.setItem('myHealth', '100');
localStorage.setItem('myMagic', 20);
print('localStorage.getItem("myHealth") = ' + localStorage.getItem('myHealth'));
print('localStorage.getItem("myMagic") = ' + localStorage.getItem('myMagic'));
localStorage.removeItem('myMagic');
print('localStorage.getItem("myMagic") = ' + localStorage.getItem('myMagic'));
localStorage.clear();
print('localStorage.getItem("myHealth") = ' + localStorage.getItem('myHealth'));
