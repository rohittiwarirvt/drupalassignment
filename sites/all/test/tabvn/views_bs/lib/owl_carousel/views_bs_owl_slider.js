(function ($) {
  Drupal.behaviors.views_bs_owl = {
    attach: function (context, settings) {
      $.each(settings.viewsBs.owl_slider, function (id, owl_slider) {
        var $container = $('#views-bs-owl-' + owl_slider.id);
         console.log(owl_slider.owl);
		 console.log($container.attr('id') +': '+$container.width());
		 console.log($(window).width());
        var $item_height = $container.find('.views-bs-owl-slider-item').height();
        $container.owlCarousel(owl_slider.owl);
		
		
		
      });


    }

  };

})(jQuery);