type articlesState =
  | LoadingArticles
  | ErrorFetchingArticles
  | LoadedArticles(array(Types.article));

[@react.component]
let make = () => {
  let (state, setState) = React.useState(() => LoadingArticles);

  React.useEffect0(() => {
    let payload = Js.Dict.empty();
    Js.Dict.set(payload, "limit", Js.Json.number(10.0));
    Js.Dict.set(payload, "offset", Js.Json.number(0.0));
    Js.Promise.(
      Fetch.fetchWithInit(
        "https://blog.erl.net.ua/api/v1/post/get_available",
        Fetch.RequestInit.make(
          ~method_=Post,
          ~body=
            Fetch.BodyInit.make(
              Js.Json.stringify(Js.Json.object_(payload)),
            ),
          ~headers=
            Fetch.HeadersInit.make({"Content-Type": "application/json"}),
          (),
        ),
      )
      |> then_(Fetch.Response.text)
      |> then_(items => {
           let articles: array(Types.article) =
             items |> Json.parseOrRaise |> Utils.DecodeArticles.articles;
           setState(_prev => LoadedArticles(articles));
           articles |> resolve;
         })
      |> ignore
    );

    None;
  });

  <div
    style={ReactDOMRe.Style.make(
      ~height="120px",
      ~display="flex",
      ~alignItems="center",
      ~justifyContent="center",
      (),
    )}>
    {switch (state) {
     | ErrorFetchingArticles => React.string("An error occurred!")
     | LoadingArticles => React.string("Loading...")
     | LoadedArticles(articles) =>
       articles
       ->Belt.Array.map(article => {
           <div key={article.id}>
             <p> {React.string(article.title)} </p>
           </div>
         })
       ->React.array
     }}
  </div>;
};