~of = Server.remote(\of, NetAddr("localhost", 9993));

// JShaderS uses JEventDynamic and therefore busses[0] is reversed for the Event-id. 
x = {JShaderS.kr(0, rgba: [255, 255, 255, SinOsc.ar(1).abs], busses: [8, SinOsc.ar(1).abs]); 1; }.play(~of);
b = Buffer.alloc(~of, 100);
b.setn(0, "../../../libs/shaders/shaderTest3".ascii);
y = {JBufferSender.kr(b, 1, x.nodeID, 0); EnvGate.new(1, Impulse.kr(0), 1)}.play(~of) // Load the shader...

b.setn(0, 0!100); // Reset buffer

~of.plotTree
x.free

// Test with Ndefs. Have to reload each time... 
Ndef(\a -> \of, {JShaderS.kr(0, rgba: [255, 0, 255, SinOsc.ar(1).abs], busses: [8, SinOsc.ar(1/4).range(0.2, 0.8), LFDNoise1.ar(1).abs,SinOsc.ar(1/4).abs, 1.0]); 1; }).play;
y = {JBufferSender.kr(b, 1, Ndef(\a->\of).objects[0].nodeID, 0); EnvGate.new(1, Impulse.kr(0), 1)}.play(~of) // Load the shader...
