let concatArray:
  (
    ~scheduler: RxPriorityScheduler.t=?,
    array(RxObservable.t('a))
  ) =>
  RxObservable.t('a);

let concatList:
  (
    ~scheduler: RxPriorityScheduler.t=?,
    list(RxObservable.t('a))
  ) =>
  RxObservable.t('a);

let debounceTime: (float, RxObservable.t('a)) => RxObservable.t('a);

let distinctUntilChanged: RxObservable.t('a) => RxObservable.t('a);

let ignoreElements: RxObservable.t('a) => RxObservable.t('b);

let keep: ('a => bool, RxObservable.t('a)) => RxObservable.t('a);

let map: ('a => 'b, RxObservable.t('a)) => RxObservable.t('b);
let map1: (('ctx0, 'a) => 'b, 'ctx0, RxObservable.t('a)) => RxObservable.t('b);
let map2: (('ctx0, 'ctx1, 'a) => 'b, 'ctx0, 'ctx1, RxObservable.t('a)) => RxObservable.t('b);

let mergeArray:
  (
    ~scheduler: RxPriorityScheduler.t=?,
    ~maxConcurrency: int=?,
    array(RxObservable.t('a))
  ) =>
  RxObservable.t('a);

let mergeList:
  (
    ~scheduler: RxPriorityScheduler.t=?,
    ~maxConcurrency: int=?,
    list(RxObservable.t('a))
  ) =>
  RxObservable.t('a);

let mergeMap:
  (~maxConcurrency: int=?, 'a => RxObservable.t('b), RxObservable.t('a)) =>
  RxObservable.t('b);

let observe:
  (
    ~onNext: 'a => unit,
    ~onError: exn => unit,
    ~onComplete: unit => unit,
    RxObservable.t('a)
  ) =>
  RxObservable.t('a);

let observe1:
  (
    ~onNext: ('ctx0, 'a) => unit,
    ~onError: ('ctx0, exn) => unit,
    ~onComplete: ('ctx0) => unit,
    'ctx0,
    RxObservable.t('a)
  ) =>
  RxObservable.t('a);

let onNext: ('a => unit, RxObservable.t('a)) => RxObservable.t('a);

let share: RxObservable.t('a) => RxObservable.t('a);

let shareReplayLast: RxObservable.t('a) => RxObservable.t('a);

let skip: (int, RxObservable.t('a)) => RxObservable.t('a);

let startWith: ('a, RxObservable.t('a)) => RxObservable.t('a);

let switchMap:
  ('a => RxObservable.t('b), RxObservable.t('a)) => RxObservable.t('b);

let take: (int, RxObservable.t('a)) => RxObservable.t('a);

let takeUntil:
  (RxObservable.t('b), RxObservable.t('a)) => RxObservable.t('a);

let withLatestFrom:
  (RxObservable.t('b), RxObservable.t('a)) => RxObservable.t(('a, 'b));