type t;

module rec Continuation: {type t = (~shouldYield: unit => bool) => Result.t;}
and Result: {
  type t;

  let yield: Continuation.t => t;
  let complete: t;
};

let immediate: t;
let userBlocking: t;
let normal: t;
let low: t;
let idle: t;

let schedule: (Continuation.t, t) => RxDisposable.t;