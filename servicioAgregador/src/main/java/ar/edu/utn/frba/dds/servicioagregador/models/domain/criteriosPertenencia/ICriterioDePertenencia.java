package ar.edu.utn.frba.dds.servicioagregador.models.domain.criteriosPertenencia;

import ar.edu.utn.frba.dds.servicioagregador.models.domain.Hecho;

public interface ICriterioDePertenencia {

  public Boolean cumpleCriterio(Hecho hecho);
}