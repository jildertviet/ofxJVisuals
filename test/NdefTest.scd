~of = Server.remote(\of, NetAddr("localhost", 9993));

(
Ndef(\a -> \of, {
  var env = Env.adsr(1e-3, 1e-4, 1, \fadeTime.kr(1)).ar(gate: \gate.kr(1), doneAction: 2);
  JRectangleS.kr((0..2), 
    loc: [\x.kr(0).lag(1), 100  * TRand.kr(0, 8, 1!3), 0],
    size: [100, 100, 0], 
    rgba: [255, 255, 255, \br.kr(1) * env]); 1;
  }
);
)
Ndef(\a -> \of)[1] = \pset -> Pbind(\fadeTime, 4, \delta, 1, \x, Pwhite(0, 1000, inf)); // See NodeProxyRoles Help
// \xset creates new object. fadeTime is not visible... 

Ndef(\a -> \of)[1] = nil; // Stop Pset

Ndef(\a -> \of).fadeTime = 1
Ndef(\b -> \of, {SinOsc.kr(0.5).range(0.0, 1.0)}).play;
Ndef(\b -> \of, {Saw.kr(1)}).play;
Ndef(\a->\of).map(\br, Ndef(\b->\of));

Ndef.clear()

~of.freeAll
~of.plotTree
