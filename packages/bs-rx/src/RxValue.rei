/**
 * Stateful values whose changes can be observed.
 */;
type t('a);

include RxDisposableLike.S1 with type t('a) := t('a);
include RxValueLike.S1 with type t('a) := t('a);

let create: 'a => t('a);