package ar.edu.utn.frba.dds.servicioagregador.controllers;

import ar.edu.utn.frba.dds.servicioagregador.models.dtos.input.ColeccionInputDTO;
import ar.edu.utn.frba.dds.servicioagregador.models.dtos.output.ColeccionOutputDTO;
import ar.edu.utn.frba.dds.servicioagregador.services.impl.ColeccionService;
import org.springframework.web.bind.annotation.CrossOrigin;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/servicioAgregacion/colecciones")
@CrossOrigin(origins = "http://localhost:8083")
public class ColeccionController {
  private ColeccionService coleccionService;

  public ColeccionController(ColeccionService coleccionService) {
    this.coleccionService = coleccionService;
  }

  @PostMapping
  public ColeccionOutputDTO crearColeccion(@RequestBody ColeccionInputDTO coleccionDTO) {
    return this.coleccionService.crearColeccion(coleccionDTO);
  }
}
