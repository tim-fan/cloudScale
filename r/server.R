library(shiny)
library(jsonlite)
library(lubridate)
library(dplyr)

# Define server logic required to draw a histogram
shinyServer(function(input, output, session) {
  
  getWeightData <- function(){
    #get weight data from phant:
    phantUrl <- 'http://data.sparkfun.com/output/mKEvnrmWD0iKXbbN76r3.json'
    weightTable <- fromJSON(phantUrl)
    weightTable <- weightTable %>% 
      arrange(desc(timestamp)) %>% 
      mutate(timestamp = suppressMessages(ymd_hms(weightTable$timestamp, tz = "Pacific/Auckland")))
    weightTable
  }
  lastWeightData <- getWeightData
  
  getWeightDataSafely <- function(){
    #wraps 'getWeightData', returns previous value if an error is caught
    tryCatch(
      {
        lastWeightData <- getWeightData()
        lastWeightData
      },
      error = function(e) {
        cat("caught an error")
        return(lastWeightData)}
    )
    
  }
  
  
  currentWeightData <- reactivePoll(10000, session, checkFunc = getWeightDataSafely, valueFunc = getWeightDataSafely)
  
  output$weightPlot <- renderPlotly({
    plot_ly(data = currentWeightData() , x = timestamp, y = weight, line = list(shape = "linear"), mode = "lines+markers"
            
            ) %>% layout(
              paper_bgcolor='rgba(0,0,0,0)',
              plot_bgcolor='rgba(0,0,0,0)'
            )
  })
  
  output$currentWeightSummary <- renderUI({
    mostRecentWeightReading <- 
      currentWeightData() %>%
      filter(row_number() == n())
    
    currentWeight <-  mostRecentWeightReading$weight
    summaryHtml <- paste("WEIGHT", strong(currentWeight), " kg (0% fat) BMI ", strong("23.56"))
    cat(summaryHtml)
    summaryHtml
  })
})
