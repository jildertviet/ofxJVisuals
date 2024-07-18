~of = Server.remote(\of, NetAddr("localhost", 9993));

// JShaderS uses JEventDynamic and therefore busses[0] is reserved for the Event-id (8 in this case)
x = {JShaderS.kr(0, rgba: [255, 255, 255, 1.0], busses: [8, SinOsc.ar(rrand(0.1, 0.2)).abs, 1.0, 1.0]); 1; }.play(~of);
b = Buffer.alloc(~of, 100);
b.setn(0, "../../../libs/shaders/shaderTest3".ascii);
y = {JBufferSender.kr(b, 1, x.nodeID, 0); EnvGate.new(1, Impulse.kr(0), 1)}.play(~of) // Load the shader...

b.setn(0, 0!100); // Reset buffer

~of.plotTree
x.free

// Test with Ndefs. Have to reload each time... 
Ndef(\a -> \of, {JShaderS.kr(0, rgba: [255, 0, 255, SinOsc.ar(1).abs], busses: [8, SinOsc.ar(1/4).range(0.2, 0.8), LFDNoise1.ar(1).abs,SinOsc.ar(1/4).abs, 1.0]); 1; }).play;
y = {JBufferSender.kr(b, 1, Ndef(\a->\of).objects[0].nodeID, 0); EnvGate.new(1, Impulse.kr(0), 1)}.play(~of) // Load the shader...

r = {JRectangleS.kr(0, loc: [0, 0, 0], size: [100, 100, 0]); 1;}.play(~of);

e.free
e = {JShaderS.kr(0, layer: 3, rgba: [255, 255, 255, 1], busses: [8, SinOsc.ar(1).abs]); 1; }.play(~of);
g = Buffer.alloc(~of, 100);
g.setn(0, "../../../libs/shaders/passthrough".ascii); // Doesn't work?
y = {JBufferSender.kr(g, 1, e.nodeID, 0); EnvGate.new(1, Impulse.kr(0), 1)}.play(~of) // Load the shader...
x.free
~of.freeAll

// Test second FX shader
e = {JShaderS.kr(0, layer: 3, rgba: [255, 255, 255, 1], busses: [8, SinOsc.ar(1).abs]); 1; }.play(~of);
g = Buffer.alloc(~of, 100);
g.setn(0, "../../../libs/shaders/addred".ascii); // Doesn't work?
y = {JBufferSender.kr(g, 1, e.nodeID, 0); EnvGate.new(1, Impulse.kr(0), 1)}.play(~of) // Load the shader...
e.free
s.plotTree

// Two rectangles, as subject for the FX shaders above
x = {JRectangleS.kr(0, loc: [0, 0, 0], size: [200, 200, 0], rgba: [125, 0, 0, 1], busses: [1, 1]); 1; }.play(~of);
x = {JRectangleS.kr(0, layer: 9, loc: [100, 100, 0], size: [200, 200, 0]); 1; }.play(~of);
