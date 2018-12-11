type t;
type disposable = t;

include RxDisposableLike.S with type t := t;

module type S = {
  type t;

  let asDisposable: t => disposable;
};

module type S1 = {
  type t('a);

  let asDisposable: t('a) => disposable;
};

let addDisposable: (t, t) => t;

let addTeardown: ((unit => unit), t) => t;

let create: unit => t;

let createWithTeardown: (unit => unit) => t;

let disposed: t;

let removeDisposable: (t, t) => t;
