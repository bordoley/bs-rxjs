module Operator = {
  type t('a, 'b);
};

[@bs.send.pipe: RxObservable.t('a)]
external pipe: Operator.t('a, 'b) => RxObservable.t('b) = "pipe";

[@bs.val] [@bs.module "rxjs/operators"]
external debounceTimeOperator: float => Operator.t('a, 'a) = "debounceTime";
let debounceTime = (delay, observable) =>
  observable |> pipe(debounceTimeOperator(delay));

[@bs.val] [@bs.module "rxjs/operators"]
external distinctUntilChangedOperator: unit => Operator.t('a, 'a) =
  "distinctUntilChanged";
let distinctUntilChanged = observable =>
  observable |> pipe(distinctUntilChangedOperator());

[@bs.val] [@bs.module "rxjs/operators"]
external ignoreElementsOperator: unit => Operator.t('a, 'b) =
  "ignoreElements";
let ignoreElements = observable =>
  observable |> pipe(ignoreElementsOperator());

[@bs.val] [@bs.module "rxjs/operators"]
external keepOperator: ([@bs.uncurry] ('a => bool)) => Operator.t('a, 'a) =
  "filter";
let keep = (predicate, observable) =>
  observable |> pipe(keepOperator(predicate));

[@bs.val] [@bs.module "rxjs/operators"]
external mapOperator: ([@bs.uncurry] ('a => 'b)) => Operator.t('a, 'b) =
  "map";
let map = (mapper, observable) => observable |> pipe(mapOperator(mapper));
let map1 = (mapper, ctx0, observable) => {
  let mapper = next => mapper(ctx0, next);
  map(mapper, observable)
};
let map2 = (mapper, ctx0, ctx1, observable) => {
  let mapper = next => mapper(ctx0, ctx1, next);
   map(mapper, observable)
};

[@bs.val] [@bs.module "rxjs/operators"]
external mergeMapOperator:
  ([@bs.uncurry] ('a => RxObservable.t('b)), option(unit), option(int)) =>
  Operator.t('a, 'b) =
  "mergeMap";
let mergeMap = (~maxConcurrency=?, mapper, observable) =>
  observable |> pipe(mergeMapOperator(mapper, None, maxConcurrency));

let mergeArray = (~scheduler=?, ~maxConcurrency=?, observables) =>
  RxObservable.ofArray(~scheduler?, observables)
  |> mergeMap(~maxConcurrency?, a => a);

let concatArray = (~scheduler=?, observables) =>
  mergeArray(~scheduler?, ~maxConcurrency=1, observables);

let mergeList = (~scheduler=?, ~maxConcurrency=?, observables) =>
  RxObservable.ofList(~scheduler?, observables)
  |> mergeMap(~maxConcurrency?, a => a);

let concatList = (~scheduler=?, observables) =>
  mergeList(~scheduler?, ~maxConcurrency=1, observables);

[@bs.val] [@bs.module "rxjs/operators"]
external observeOperator:
  (
    [@bs.uncurry] ('a => unit),
    [@bs.uncurry] (exn => unit),
    [@bs.uncurry] (unit => unit)
  ) =>
  Operator.t('a, 'b) =
  "tap";
// FIXME: Need to wrap the exception from JS in an ocaml exception
let observe = (~onNext, ~onError, ~onComplete, observable) =>
  observable |> pipe(observeOperator(onNext, onError, onComplete));

let observe1 = (~onNext, ~onError, ~onComplete, ctx0, observable) => {
  let onNext = next => onNext(ctx0, next);
  let onError = exn => onError(ctx0, exn);
  let onComplete = () => onComplete(ctx0);

  observe(~onNext, ~onError, ~onComplete, observable);
};

[@bs.val] [@bs.module "rxjs/operators"]
external onNextOperator: ([@bs.uncurry] ('a => unit)) => Operator.t('a, 'b) =
  "tap";
let onNext = (onNext, observable) =>
  observable |> pipe(onNextOperator(onNext));

[@bs.val] [@bs.module "rxjs/operators"]
external shareOperator: unit => Operator.t('a, 'a) = "share";
let share = observable => observable |> pipe(shareOperator());

[@bs.val] [@bs.module "rxjs/operators"]
external shareReplayOperator: int => Operator.t('a, 'a) = "shareReplay";
let shareReplayLast = observable =>
  observable |> pipe(shareReplayOperator(1));

[@bs.val] [@bs.module "rxjs/operators"]
external skipOperator: int => Operator.t('a, 'b) = "skip";
let skip = (count, observable) => observable |> pipe(skipOperator(count));

[@bs.val] [@bs.module "rxjs/operators"]
external startWithOperator: 'a => Operator.t('a, 'a) = "startWith";
let startWith = (value, observable) =>
  observable |> pipe(startWithOperator(value));

[@bs.val] [@bs.module "rxjs/operators"]
external switchMapOperator:
  ([@bs.uncurry] ('a => RxObservable.t('b))) => Operator.t('a, 'b) =
  "switchMap";
let switchMap = (mapper, observable) =>
  observable |> pipe(switchMapOperator(mapper));

[@bs.val] [@bs.module "rxjs/operators"]
external takeOperator: int => Operator.t('a, 'b) = "take";
let take = (count, observable) => observable |> pipe(takeOperator(count));

[@bs.val] [@bs.module "rxjs/operators"]
external takeUntilOperator: RxObservable.t('b) => Operator.t('a, 'a) =
  "takeUntil";
let takeUntil = (mapper, observable) =>
  observable |> pipe(takeUntilOperator(mapper));

[@bs.val] [@bs.module "rxjs/operators"]
external withLatestFromOperator:
  RxObservable.t('b) => Operator.t('a, ('a, 'b)) =
  "withLatestFrom";
let withLatestFrom = (observableB, observableA) =>
  observableA |> pipe(withLatestFromOperator(observableB));