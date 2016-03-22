library(shiny)
library(plotly)
library(shinythemes)
library(shinydashboard)

dashboardPage(
  dashboardHeader(title = "Weight Dashboard"),
  # dashboardHeader(disable = TRUE),
  dashboardSidebar(disable = TRUE),
  dashboardBody(
    # box(
      fluidRow(
        div(style = 
             "height:50px;
              padding-right: 15px;
              padding-left: 15px;",
          actionButton("goButton", "Week"), 
          actionButton("goButton", "Month"),
          actionButton("goButton", "Year"),
          actionButton("goButton", "All")
#        ),
#        div(
#          style =
#             "height:50px;
#              padding-right: 15px;
#              padding-left: 15px;",
#          dateInput("date", "Date:",
#                  startview = "decade", format = "yy"
#          )
        )
      ),
      fluidRow(
#          box(
            plotlyOutput("weightPlot")
#            width = "100%"
#          )
      )
    )
  # )
    # box(plotlyOutput("weightPlot"))))
)
#' 
#' # Define UI for application that draws a histogram
#' shinyUI(fluidPage(
#'   #theme = shinytheme("journal"),
#'   dashboardHeader("test"),
#'   tags$head(
#'     tags$style(HTML("
#'       @import url('//fonts.googleapis.com/css?family=Lobster|Cabin:400,700');
#'       @import url('//fonts.googleapis.com/css?family=Indie+Flower');
#' 
#'       h2 {
#'         font-family: 'Lobster', cursive;
#'         font-weight: 500;
#'         line-height: 1.1;
#'         color: #ffffff;
#'         background-color: #808080;
#'         padding-top: 0px;
#'         padding-right: 0px;
#'         padding-bottom: 0px;
#'         padding-left: 0px;
#'         margin-top: 0px;
#'       };
#'       .container-fluid {
#'         padding-right: 0px;
#'         padding-left: 0px !important; 
#'         background-color: #808080;
#'       }
#' 
#'     "))),
#'   
#'   # Application title
#'   titlePanel(div(p("Weight Graph"))),
#'   
#'   # Show a plot of the generated distribution
#'   mainPanel(
#'     htmlOutput("currentWeightSummary")),
#'     plotlyOutput("weightPlot")
#'   )
#' )
