
let lang = 'EN';

document.addEventListener("load", function () {
  var i = 0;
  document.querySelector("#home-content h2").textContent = content[0].title;
  document.querySelector("#home-content div").innerHTML = content[0]['content'][lang];

    var swiper = new Swiper(".mySwiper", {
        effect: "cube",
        grabCursor: true,
        cubeEffect: {
          shadow: true,
          slideShadows: true,
          shadowOffset: 20,
          shadowScale: 0.94,
        },
        pagination: {
          el: ".swiper-pagination",
        },
        on: {
          slideChange(s) {
            const index = s.activeIndex;
            document.querySelector("#home-content h2").textContent = content[index].title;
            document.querySelector("#home-content div").innerHTML = content[index]['content'][lang];
            i = index;
          },
        },
    });


    document.querySelectorAll("header .dropdown a.dropdown-item").forEach(link => {
      link.addEventListener("click", function (e) {
        e.preventDefault();
        document.querySelectorAll("header .nav-link").forEach(l => {
          try {
            l.textContent = headerLinks[l.getAttribute("href").substring(1)][lang];
          } catch (error) {
            
          }
        })


        innerHTML = link.innerHTML;
        document.querySelector("header .dropdown-toggle").innerHTML = innerHTML;
        lang = link.dataset.lang
        document.querySelector("#home-content h2").textContent = content[i].title;
        document.querySelector("#home-content div").innerHTML = content[i]['content'][lang];
        Object.keys(headerLinks).forEach(l => {
          try {
            document.querySelector(`#${l} .sub-title`).textContent = headerLinks[l][lang];
          } catch (error) {
            
          }
        });

      })
    })
}, true)