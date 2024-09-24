~of = Server.remote(\of, NetAddr("localhost", 9993));

// JShaderS uses JEventDynamic and therefore busses[0] is reserved for the Event-id (8 in this case)
(
x = {JShaderS.kr(0, rgba: [255, 255, 255, 1.0], numBusses: 21, busses: 
[8, 
  // LFTri.kr(0.4).range(0.1, 0.2),
  0.2,
  0.2, 0.5, 0.5,
  0.0, 0.0, 0.0,
  1.0, 1.0, 1.0
], mode: 0
); 1; }.play(~of);
)
b = Buffer.alloc(~of, 100);
b.setn(0, "/home/jildert/of_v0.12.0-rc1_linux64gcc6_release/addons/ofxJVisuals/test/gradientRect".ascii);
y = {JBufferSender.kr(b, 1, x.nodeID, 0); EnvGate.new(1, Impulse.kr(0), 1)}.play(~of) // Load the shader...

b.setn(0, 0!100); // Reset buffer

~of.plotTree
~of.freeAll;
x.free

// Test with Ndefs. Have to reload each time... 
(
Ndef(\a -> \of, {
  JShaderS.kr(0, rgba: [255, 0, 255, 1.0], numBusses: 21, busses: 
  [
    8, 
    // Env.perc(0.1, 0.4, level: LFDNoise1.kr(1).abs).kr(gate: Impulse.kr(1)), 
    // 0.0,
   LFTri.kr(0.4).range(0.1, 0.2),
    0.0, 
    1.0, 
    1.0,
    1.0, 1.0, 1.0,
    0.0, 0.0, 1.0
  ], mode: 0, size: [100, 100, 0]
); 1; }).play;
)
s.plotTree
y = {JBufferSender.kr(b, 1, Ndef(\a->\of).objects[0].nodeID, 0); EnvGate.new(1, Impulse.kr(0), 1)}.play(~of);//  Load the shader...
[0,1,2].at((0..2))
