type t('a);
 
include RxDisposableLike.S1 with type t('a) := t('a);
include RxObservable.S1 with type t('a) := t('a);

let create: unit => t('a);
let dispatch: ('a, t('a)) => unit;