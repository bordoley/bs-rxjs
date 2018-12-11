module Operator = {
  type t('a, 'b);
};

type error;

[@bs.send.pipe: RxObservable.t('a)]
external pipe: Operator.t('a, 'b) => RxObservable.t('b) = "pipe";

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external debounceTimeOperator: float => Operator.t('a, 'a) = "debounceTime";
let debounceTime = (delay, observable) =>
  observable |> pipe(debounceTimeOperator(delay));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external distinctUntilChangedOperator: unit => Operator.t('a, 'a) =
  "distinctUntilChanged";
let distinctUntilChanged = observable =>
  observable |> pipe(distinctUntilChangedOperator());

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external ignoreElementsOperator: unit => Operator.t('a, 'b) =
  "ignoreElements";
let ignoreElements = observable =>
  observable |> pipe(ignoreElementsOperator());

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external keepOperator: ([@bs.uncurry] ('a => bool)) => Operator.t('a, 'a) =
  "filter";
let keep = (predicate, observable) =>
  observable |> pipe(keepOperator(predicate));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external mapOperator: ([@bs.uncurry] ('a => 'b)) => Operator.t('a, 'b) =
  "map";
let map = (mapper, observable) => observable |> pipe(mapOperator(mapper));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
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

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external observeOperator:
  (
    [@bs.uncurry] ('a => unit),
    [@bs.uncurry] (error => unit),
    [@bs.uncurry] (unit => unit)
  ) =>
  Operator.t('a, 'b) =
  "tap";
let observe = (~onNext, ~onError, ~onComplete, observable) =>
  observable |> pipe(observeOperator(onNext, onError, onComplete));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external onNextOperator: ([@bs.uncurry] ('a => unit)) => Operator.t('a, 'b) =
  "tap";
let onNext = (onNext, observable) =>
  observable |> pipe(onNextOperator(onNext));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external shareOperator: unit => Operator.t('a, 'a) = "share";
let share = observable => observable |> pipe(shareOperator());

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external shareReplayOperator: int => Operator.t('a, 'a) = "shareReplay";
let shareReplayLast = observable =>
  observable |> pipe(shareReplayOperator(1));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external skipOperator: int => Operator.t('a, 'b) = "skip";
let skip = (count, observable) => observable |> pipe(skipOperator(count));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external startWithOperator: 'a => Operator.t('a, 'a) = "startWith";
let startWith = (value, observable) =>
  observable |> pipe(startWithOperator(value));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external switchMapOperator:
  ([@bs.uncurry] ('a => RxObservable.t('b))) => Operator.t('a, 'b) =
  "switchMap";
let switchMap = (mapper, observable) =>
  observable |> pipe(switchMapOperator(mapper));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external takeOperator: int => Operator.t('a, 'b) = "take";
let take = (count, observable) => observable |> pipe(takeOperator(count));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external takeUntilOperator: RxObservable.t('b) => Operator.t('a, 'a) =
  "takeUntil";
let takeUntil = (mapper, observable) =>
  observable |> pipe(takeUntilOperator(mapper));

[@bs.val] [@bs.scope "Operator"] [@bs.module "rxjs"]
external withLatestFromOperator:
  RxObservable.t('b) => Operator.t('a, ('a, 'b)) =
  "withLatestFrom";
let withLatestFrom = (observableB, observableA) =>
  observableA |> pipe(withLatestFromOperator(observableB));