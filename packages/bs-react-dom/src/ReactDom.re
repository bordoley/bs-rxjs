[@bs.val] [@bs.module "react-dom"]
external render: (React.Element.t, Dom.element) => unit = "render";

[@bs.val] [@bs.return nullable]
external _getElementById: string => option(Dom.element) =
  "document.getElementById";

let renderToElementWithId = (reactElement, id) =>
  switch (_getElementById(id)) {
  | None =>
    raise(
      Invalid_argument(
        "ReactDom.renderToElementWithId : no element of id "
        ++ id
        ++ " found in the HTML.",
      ),
    )
  | Some(element) => render(reactElement, element)
  };

external string: string => React.Element.t = "%identity";

let a = ReactDomComponent.create("a");
let button = ReactDomComponent.create("button");
let div = ReactDomComponent.create("div");
let h1 = ReactDomComponent.create("h1");
let h2 = ReactDomComponent.create("h2");
let h3 = ReactDomComponent.create("h3");
let h4 = ReactDomComponent.create("h4");
let h5 = ReactDomComponent.create("h5");
let h6 = ReactDomComponent.create("h6");
let img = ReactDomComponent.create("img");
let input = ReactDomComponent.create("input");
let label = ReactDomComponent.create("label");
let li = ReactDomComponent.create("li");
let ol = ReactDomComponent.create("ol");
let span = ReactDomComponent.create("span");
let table = ReactDomComponent.create("table");
let td = ReactDomComponent.create("td");
let textarea = ReactDomComponent.create("textarea");
let tfoot = ReactDomComponent.create("tfoot");
let th = ReactDomComponent.create("th");
let thead = ReactDomComponent.create("thead");
let ul = ReactDomComponent.create("ul");