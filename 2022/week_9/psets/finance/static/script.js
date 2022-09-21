$(document).ready(function() {
    $("#sell_stock_select").on("change", function() {
        let symbol = $(this).val()

        if (symbol == "" || !symbol) // No symbol selected
        {
            // could use this.disabled = true;
            $("#shares_tobe_sold").val("");
            $("#shares_tobe_sold").prop("disabled", true);
            $("#shares_tobe_sold").prop("min", "0");
            $("#shares_tobe_sold").prop("max", "0");
        } else {
            // Get the shares value of the stock

            // Creating a new AJAX object to asynchronously (without reloading) update the page
            $.ajax({
                    url: "/get-shares",
                    data: {
                        symbol: symbol
                    },
                    type: "GET",
                    datatype: "json"
                })

                // If the request succeeds, the response is passed to the function
                .done(function(shares) {
                    $("#shares_tobe_sold").prop("min", 1);
                    $("#shares_tobe_sold").prop("max", shares["max"]);
                })

                // Code to run if the request fails; the raw request and
                // status codes are passed to the function
                .fail(function(xhr, status, errorThrown) {
                    alert("Sorry, there was a problem!");
                    console.log("Error: " + errorThrown);
                    console.log("Status: " + status);
                    console.dir(xhr);
                })

                // Code to run regardless of success or failure;
                .always(function(xhr, status) {
                    console.log("The request is complete!");
                });

            // Enable the numeric input and change the max value of it
            $("#shares_tobe_sold").prop("disabled", false);
        }
    });
});