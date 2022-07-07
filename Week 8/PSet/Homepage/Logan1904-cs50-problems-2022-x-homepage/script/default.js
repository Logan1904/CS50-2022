document.addEventListener("DOMContentLoaded", function() {

    // When CS50 button is clicked, go to CS50 coursepage
    let course_button = document.querySelector("#button_cs50");
    course_button.addEventListener("click", function() {
        window.open("https://cs50.harvard.edu/x/2022/", '_blank').focus();
    })

    // When Solutions button is clicked, go to my GitHub repository for CS50 solutions
    let github_button = document.querySelector('#button_solutions');
    github_button.addEventListener("click", function() {
        window.open("https://github.com/Logan1904/CS50-2022", '_blank').focus();
    })
});