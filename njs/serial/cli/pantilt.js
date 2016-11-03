
// TODO: use event handlers to manage arrival of serial data

var sys = require('../req');
var req = sys.req;
var ink = sys.ink;

var serialport = req('serialport');
var program    = req('commander');

sys.ready('M O T O R  C O R T E X   I N I T I A L I Z E D');

function print(msg) {
  console.log(msg);
}

program
  .version('0.2.0')
  .usage('<device> <pan> <tilt> [options]')
  .option('-d, --debug', 'Debug')
  .option('-s, --sim', "Don't send to serial port, only simulate")
  .parse(process.argv);

if(program.args.length !== 3) program.help();

if (program.sim) {
  print('SIMULATION ONLY');
}
if (program.debug) {
  print('DEBUGGING');
}

portName  = process.argv[2];
pan       = process.argv[3];
tilt      = process.argv[4];


var timer;
var delay = 500;

var ackSerial   = 0,
    interrupt   = 0,
    serialUp    = 0,  // serial port is open
    serialData  = 0;  // serial data was read from remote system

if(!program.sim) {
  var port = new serialport(portName, {
     baudRate: 115200,
     // look for return and newline at the end of each data packet:
     parser: serialport.parsers.readline("\n")
   });

  port.on('open', serialOnOpen);
  port.on('data', serialOnData);
  port.on('close', serialOnClose);
  port.on('error', serialOnErr);
}




function serialOnOpen()
{
  print(ink.itag+' '+portName+' open @: ' + port.options.baudRate + ' '+ink.brkt('OK'.green.bold));
}


function serialOnData(data)
{
  serialData = 1;

  print(data+' '+ink.brkt('OK'.green.bold));

  if (data.indexOf("Listening for serial commands...") > -1) {
    serialUp = 1;
    port.write(pan+'_'+tilt+'\n');
    // process.exit();
  }
}


function serialOnClose()
{
   print(ink.wtag+' port closed.');
}


function serialOnErr(err)
{
   print(ink.etag+' serial port error: ' + err);
}
