type t('a);

include RxDisposableLike.S1 with type t('a) := t('a);
include RxDisposable.S1 with type t('a) := t('a);

let asDisposable: t('a) => RxDisposable.t;
let complete: (t('a)) => unit;
let next: ('a, t('a)) => unit;
let error: (exn, t('a)) => unit;
