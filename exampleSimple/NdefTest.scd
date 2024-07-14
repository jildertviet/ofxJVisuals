~of = Server.remote(\of, NetAddr("localhost", 9993));

Ndef(\a -> \of, {JRectangleS.kr(0, loc: [100, 100, 0], size: [100, 100, 0], rgba: [255, 255, 255, \br.kr(1)]); 1});
Ndef(\b -> \of, {SinOsc.kr(0.5).range(0.0, 1.0)}).play;
Ndef(\b -> \of, {Saw.kr(1)}).play;
Ndef(\a->\of).map(\br, Ndef(\b->\of));

Ndef.clear()

~of.freeAll
~of.plotTree
